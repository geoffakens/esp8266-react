#include <ESP8266React.h>
#include <PresenceSettingsService.h>
#include <PresenceMqttSettingsService.h>
#include <PresenceStateService.h>

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

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework
  esp8266React.begin();

  // start presence services
  presenceSettingsService.begin();
  presenceMqttSettingsService.begin();
  presenceStateService.begin();

  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();

  // Monitor sensor state
  presenceStateService.loop();
}
