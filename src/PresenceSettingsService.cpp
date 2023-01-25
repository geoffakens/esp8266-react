#include <PresenceSettingsService.h>

PresenceSettingsService::PresenceSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(PresenceSettings::read,
                  PresenceSettings::update,
                  this,
                  server,
                  PRESENCE_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(PresenceSettings::read, PresenceSettings::update, this, fs, PRESENCE_SETTINGS_FILE) {
}

void PresenceSettingsService::begin() {
  Serial.println(F("Starting presence settings service..."));
  _fsPersistence.readFromFS();
}
