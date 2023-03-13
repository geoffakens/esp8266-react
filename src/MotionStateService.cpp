#include <MotionStateService.h>

MotionStateService::MotionStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     AsyncMqttClient* mqttClient,
                                     PresenceMqttSettingsService* presenceMqttSettingsService) :
    _webSocket(MotionState::read,
               MotionState::update,
               this,
               server,
               MOTION_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPub(MotionState::read, this, mqttClient, "", true),
    _presenceMqttSettingsService(presenceMqttSettingsService) {
  mqttClient->onConnect(std::bind(&MotionStateService::applyMqttSettings, this));
  _presenceMqttSettingsService->addUpdateHandler([&](const String& originId) { onMqttSettingsUpdated(); }, false);
}

void MotionStateService::begin() {
  Serial.println(F("Starting motion state service..."));

  // set initial state
  _state.motionDetected = false;

  pinMode(SENSOR_PIN, INPUT);
}

void MotionStateService::loop() {
  bool newState = digitalRead(SENSOR_PIN);
  update([&](MotionState& state) {
    StateUpdateResult result = newState != state.motionDetected ? StateUpdateResult::CHANGED : StateUpdateResult::UNCHANGED;
    state.motionDetected = newState;
    return result;
  }, SENSOR_ORIGIN_ID);
}

void MotionStateService::applyMqttSettings() {
  String pubTopic;

  _presenceMqttSettingsService->read([&](PresenceMqttSettings& settings) {
    pubTopic = settings.mqttPath + "/motion";
  });

  _mqttPub.setPubTopic(pubTopic);
}

void MotionStateService::onMqttSettingsUpdated() {
  applyMqttSettings();
}
