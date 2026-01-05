#include <SimpleWiFiManager.h>

SimpleWiFiManager wifi;

void setup() {
  Serial.begin(115200);
  delay(1000);

  wifi.setDataMode(DATA_BLUETOOTH); // o DATA_SERIAL
  wifi.begin();
}

void loop() {
  wifi.loop();
}