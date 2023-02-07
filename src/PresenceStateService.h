#ifndef PresenceStateService_h
#define PresenceStateService_h

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <PresenceSettingsService.h>
#include <PresenceMqttSettingsService.h>
#include <SoftwareSerial.h>
#include <WebSocketTxRx.h>

#include "DFRobot_mmWave_Radar.h"

#define SENSOR_RX_PIN 13
#define SENSOR_TX_PIN 15
#define SENSOR_SERIAL_BAUD_RATE 115200
#define SENSOR_ORIGIN_ID "presence_sensor"

#define SENSOR_INPUT 16

#define PRESENCE_SETTINGS_ENDPOINT_PATH "/rest/presenceState"
#define PRESENCE_SETTINGS_SOCKET_PATH "/ws/presenceState"

class PresenceState {
 public:
  bool presenceDetected;

  static void read(PresenceState& presenceState, JsonObject& root) {
    root["presenceDetected"] = presenceState.presenceDetected;
  }

  static StateUpdateResult update(JsonObject& root, PresenceState& presenceState) {
    boolean newState = root["presenceDetected"] | false;
    if (presenceState.presenceDetected != newState) {
      presenceState.presenceDetected = newState;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }
};

class PresenceStateService : public StatefulService<PresenceState> {
 public:
  PresenceStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient,
                    PresenceSettingsService* presenceSettingsService,
                    PresenceMqttSettingsService* presenceMqttSettingsService);
  void begin();
  void loop();

 private:
  WebSocketTxRx<PresenceState> _webSocket;
  MqttPub<PresenceState> _mqttPub;
  PresenceSettingsService* _presenceSettingsService;
  PresenceMqttSettingsService* _presenceMqttSettingsService;
  SoftwareSerial _serial;
  DFRobot_mmWave_Radar _sensor;

  void applyPresenceSettings();
  void applyMqttSettings();
  void onSettingsUpdated();
  void onMqttSettingsUpdated();
};

#endif
