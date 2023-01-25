#ifndef PresenceSettingsService_h
#define PresenceSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <SettingValue.h>

#define PRESENCE_SETTINGS_FILE "/config/presenceSettings.json"
#define PRESENCE_SETTINGS_PATH "/rest/presenceSettings"

#define SENSOR_ORIGIN_ID "sensor"

class PresenceSettings {
  public:
   float minRange;
   float maxRange;
   float detectionDelay;
   float clearDelay;

  static void read(PresenceSettings& settings, JsonObject& root) {
    root["minRange"] = settings.minRange;
    root["maxRange"] = settings.maxRange;
    root["detectionDelay"] = settings.detectionDelay;
    root["clearDelay"] = settings.clearDelay;
  }

  static StateUpdateResult update(JsonObject& root, PresenceSettings& settings) {
    settings.minRange = root["minRange"];
    settings.maxRange = root["maxRange"];
    settings.detectionDelay = root["detectionDelay"];
    settings.clearDelay = root["clearDelay"];
    return StateUpdateResult::CHANGED;
  }
};

class PresenceSettingsService : public StatefulService<PresenceSettings> {
 public:
  PresenceSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<PresenceSettings> _httpEndpoint;
  FSPersistence<PresenceSettings> _fsPersistence;
};

#endif
