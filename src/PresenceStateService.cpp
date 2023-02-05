#include <PresenceStateService.h>

PresenceStateService::PresenceStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     AsyncMqttClient* mqttClient,
                                     PresenceSettingsService* presenceSettingsService,
                                     PresenceMqttSettingsService* presenceMqttSettingsService) :
    _webSocket(PresenceState::read,
               PresenceState::update,
               this,
               server,
               PRESENCE_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPub(PresenceState::read, this, mqttClient, "", true),
    _presenceSettingsService(presenceSettingsService),
    _presenceMqttSettingsService(presenceMqttSettingsService),
    _serial(SENSOR_RX_PIN, SENSOR_TX_PIN),
    _sensor(&_serial) {
  mqttClient->onConnect(std::bind(&PresenceStateService::applyMqttSettings, this));
  _presenceSettingsService->addUpdateHandler([&](const String& originId) { onSettingsUpdated(); }, false);
  _presenceMqttSettingsService->addUpdateHandler([&](const String& originId) { onMqttSettingsUpdated(); }, false);
}

void PresenceStateService::begin() {
  Serial.println(F("Starting presence state service..."));

  // set initial state
  _state.presenceDetected = false;

  // start serial communication with the sensor
  _serial.begin(SENSOR_SERIAL_BAUD_RATE);

  // reset sensor to factory defaults
  _sensor.factoryReset();

  // apply current settings
  applyPresenceSettings();

  pinMode(SENSOR_INPUT, INPUT_PULLDOWN_16);
}

void PresenceStateService::loop() {
  bool newState = _sensor.readPresenceDetection();
  update([&](PresenceState& state) {
    StateUpdateResult result = newState != state.presenceDetected ? StateUpdateResult::CHANGED : StateUpdateResult::UNCHANGED;
    state.presenceDetected = newState;
    return result;
  }, SENSOR_ORIGIN_ID);
}

void PresenceStateService::applyPresenceSettings() {
  float minRange;
  float maxRange;
  float detectionDelay;
  float clearDelay;

  _presenceSettingsService->read([&](PresenceSettings& settings) {
    minRange = settings.minRange;
    maxRange = settings.maxRange;
    detectionDelay = settings.detectionDelay;
    clearDelay = settings.clearDelay;
  });

  Serial.print("Applying sensor settings minRange=");
  Serial.print(minRange);
  Serial.print(" maxRange=");
  Serial.print(maxRange);
  Serial.print(" detectionDelay=");
  Serial.print(detectionDelay);
  Serial.print(" clearDelay=");
  Serial.println(clearDelay);

  _sensor.DetRangeCfg(minRange, maxRange);
  _sensor.OutputLatency(detectionDelay, clearDelay);
}

void PresenceStateService::applyMqttSettings() {
  String pubTopic;

  _presenceMqttSettingsService->read([&](PresenceMqttSettings& settings) {
    pubTopic = settings.mqttPath + "/state";
  });

  _mqttPub.setPubTopic(pubTopic);
}

void PresenceStateService::onSettingsUpdated() {
  ESP.restart();
}

void PresenceStateService::onMqttSettingsUpdated() {
  applyMqttSettings();
}
