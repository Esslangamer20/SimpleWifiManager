#include <SimpleWiFiManager.h>

SimpleWiFiManager wifi;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Iniciando SimpleWiFiManager...");

  // Llamar sin parámetros para WiFi automático / AP
  wifi.begin();
}

void loop() {
  wifi.loop();

  // Mensaje por Serial si RESET
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.equalsIgnoreCase("RESET")) {
      Serial.println("RESET recibido desde Serial...");
      wifi.reset();
    }
  }
}
