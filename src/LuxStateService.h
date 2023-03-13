#ifndef LuxStateService_h
#define LuxStateService_h

#include <WebSocketTxRx.h>
#include <MqttPubSub.h>
#include <PresenceMqttSettingsService.h>
#include <Wire.h>
#include <BH1750.h>

// Sensor Pins:  SCL D1, SDA D2

#define SENSOR_ORIGIN_ID "lux_sensor"
#define SENSOR_READ_INTERVAL 30000

#define MIN_LUX 0.0001
#define MAX_LUX 10000

#define LUX_SOCKET_PATH "/ws/luxState"

class LuxState {
 public:
  float lux;

  static void read(LuxState& luxState, JsonObject& root) {
    root["lux"] = luxState.lux;
  }

  static StateUpdateResult update(JsonObject& root, LuxState& luxState) {
    float newState = root["lux"] | MIN_LUX;
    if (luxState.lux != newState) {
      luxState.lux = newState <= MAX_LUX ? newState : MAX_LUX;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }
};

class LuxStateService : public StatefulService<LuxState> {
 public:
  LuxStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient,
                    PresenceMqttSettingsService* presenceMqttSettingsService);
  void begin();
  void loop();

 private:
  WebSocketTxRx<LuxState> _webSocket;
  MqttPub<LuxState> _mqttPub;
  PresenceMqttSettingsService* _presenceMqttSettingsService;
  BH1750 _sensor;
  bool _sensorInitialized;
  unsigned long _lastUpdate;

  void applyMqttSettings();
  void onMqttSettingsUpdated();
};

#endif
