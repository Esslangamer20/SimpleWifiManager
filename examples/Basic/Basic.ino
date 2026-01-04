#include <SimpleWiFiManager.h>

SimpleWiFiManager wifi;

void setup() {
  Serial.begin(115200);
  wifi.begin();
}

void loop() {
  wifi.loop();
}
