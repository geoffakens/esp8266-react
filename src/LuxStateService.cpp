#include <LuxStateService.h>

LuxStateService::LuxStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     AsyncMqttClient* mqttClient,
                                     PresenceMqttSettingsService* presenceMqttSettingsService) :
    _webSocket(LuxState::read,
               LuxState::update,
               this,
               server,
               LUX_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPub(LuxState::read, this, mqttClient, "", true),
    _presenceMqttSettingsService(presenceMqttSettingsService),
    _sensor(0x23) {
  _lastUpdate = 0;
  _sensorInitialized = false;
  mqttClient->onConnect(std::bind(&LuxStateService::applyMqttSettings, this));
  _presenceMqttSettingsService->addUpdateHandler([&](const String& originId) { onMqttSettingsUpdated(); }, false);
}

void LuxStateService::begin() {
  Serial.println(F("Starting lux state service..."));

  // set initial state
  _state.lux = MIN_LUX;

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();

  if (_sensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    _sensorInitialized = true;
  }
}

void LuxStateService::loop() {
  unsigned long currentMillis = millis();

  if (
    _sensorInitialized &&
    currentMillis - _lastUpdate >= SENSOR_READ_INTERVAL &&
    _sensor.measurementReady()
  ) {
    float lux = _sensor.readLightLevel();

    update([&](LuxState& state) {
      state.lux = lux;
      return StateUpdateResult::CHANGED;
    }, SENSOR_ORIGIN_ID);

    _lastUpdate = currentMillis;
  }
}

void LuxStateService::applyMqttSettings() {
  if (_sensorInitialized) {
    String pubTopic;

    _presenceMqttSettingsService->read([&](PresenceMqttSettings& settings) {
      pubTopic = settings.mqttPath + "/lux";
    });

    _mqttPub.setPubTopic(pubTopic);
  }
}

void LuxStateService::onMqttSettingsUpdated() {
  applyMqttSettings();
}
