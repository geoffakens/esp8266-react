#ifndef PresenceMqttSettingsService_h
#define PresenceMqttSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <SettingValue.h>

#define PRESENCE_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define PRESENCE_BROKER_SETTINGS_PATH "/rest/brokerSettings"

class PresenceMqttSettings {
 public:
  String mqttPath;
  String name;
  String uniqueId;

  static void read(PresenceMqttSettings& settings, JsonObject& root) {
    root["mqttPath"] = settings.mqttPath;
    root["name"] = settings.name;
    root["uniqueId"] = settings.uniqueId;
  }

  static StateUpdateResult update(JsonObject& root, PresenceMqttSettings& settings) {
    settings.mqttPath = root["mqttPath"] | SettingValue::format("presence-detective/#{unique_id}");
    settings.name = root["name"] | SettingValue::format("presence-detective-#{unique_id}");
    settings.uniqueId = root["uniqueId"] | SettingValue::format("presence-detective-#{unique_id}");
    return StateUpdateResult::CHANGED;
  }
};

class PresenceMqttSettingsService : public StatefulService<PresenceMqttSettings> {
 public:
  PresenceMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<PresenceMqttSettings> _httpEndpoint;
  FSPersistence<PresenceMqttSettings> _fsPersistence;
};

#endif  // end PresenceMqttSettingsService_h
