#include <ESP8266React.h>
#include <PresenceSettingsService.h>
#include <PresenceMqttSettingsService.h>
#include <PresenceStateService.h>
#include <MotionStateService.h>
#include <LuxStateService.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);
PresenceSettingsService presenceSettingsService = PresenceSettingsService(&server,
                                                        esp8266React.getFS(),
                                                        esp8266React.getSecurityManager());
PresenceMqttSettingsService presenceMqttSettingsService = PresenceMqttSettingsService(&server,
                                                        esp8266React.getFS(),
                                                        esp8266React.getSecurityManager());
PresenceStateService presenceStateService = PresenceStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &presenceSettingsService,
                                                        &presenceMqttSettingsService);
#if FT_ENABLED(FT_MOTION)
MotionStateService motionStateService = MotionStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &presenceMqttSettingsService);
#endif
#if FT_ENABLED(FT_LUX)
LuxStateService luxStateService = LuxStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &presenceMqttSettingsService);
#endif

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework
  esp8266React.begin();

  // start presence services
  presenceSettingsService.begin();
  presenceMqttSettingsService.begin();
  presenceStateService.begin();
  #if FT_ENABLED(FT_MOTION)
  motionStateService.begin();
  #endif
  #if FT_ENABLED(FT_LUX)
  luxStateService.begin();
  #endif

  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();

  // Monitor sensor state
  presenceStateService.loop();
  #if FT_ENABLED(FT_MOTION)
  motionStateService.loop();
  #endif
  #if FT_ENABLED(FT_LUX)
  luxStateService.loop();
  #endif
}
