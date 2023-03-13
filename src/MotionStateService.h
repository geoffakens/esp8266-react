#ifndef MotionStateService_h
#define MotionStateService_h

#include <MqttPubSub.h>
#include <PresenceMqttSettingsService.h>
#include <WebSocketTxRx.h>

#define SENSOR_PIN 12 // D6
#define SENSOR_ORIGIN_ID "motion_sensor"

#define MOTION_SETTINGS_ENDPOINT_PATH "/rest/motionState"
#define MOTION_SETTINGS_SOCKET_PATH "/ws/motionState"

class MotionState {
 public:
  bool motionDetected;

  static void read(MotionState& motionState, JsonObject& root) {
    root["detected"] = motionState.motionDetected;
  }

  static StateUpdateResult update(JsonObject& root, MotionState& motionState) {
    boolean newState = root["motionDetected"] | false;
    if (motionState.motionDetected != newState) {
      motionState.motionDetected = newState;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }
};

class MotionStateService : public StatefulService<MotionState> {
 public:
  MotionStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient,
                    PresenceMqttSettingsService* presenceMqttSettingsService);
  void begin();
  void loop();

 private:
  WebSocketTxRx<MotionState> _webSocket;
  MqttPub<MotionState> _mqttPub;
  PresenceMqttSettingsService* _presenceMqttSettingsService;

  void applyMotionSettings();
  void applyMqttSettings();
  void onMqttSettingsUpdated();
};

#endif
