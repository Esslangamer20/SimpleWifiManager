#include <SimpleWiFiManager.h>

SimpleWiFiManager wifi;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Cambiar modo de salida de datos: Serial o Bluetooth
  wifi.setDataMode(DATA_SERIAL); // DATA_BLUETOOTH si quieres BT
  wifi.begin();
}

void loop() {
  wifi.loop();
}
