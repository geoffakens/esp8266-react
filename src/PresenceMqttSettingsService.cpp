#include <PresenceMqttSettingsService.h>

PresenceMqttSettingsService::PresenceMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(PresenceMqttSettings::read,
                  PresenceMqttSettings::update,
                  this,
                  server,
                  PRESENCE_BROKER_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(PresenceMqttSettings::read,PresenceMqttSettings::update, this, fs, PRESENCE_BROKER_SETTINGS_FILE) {
}

void PresenceMqttSettingsService::begin() {
  Serial.println(F("Starting presence MQTT settings service..."));
  _fsPersistence.readFromFS();
}
