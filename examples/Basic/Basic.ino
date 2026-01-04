#include <SimpleWiFiManager.h>

SimpleWiFiManager wifi;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando SimpleWiFiManager...");

  // Inicia WiFi (puedes poner SSID y password)
  wifi.begin();
}

void loop() {
  // Mantiene WiFi activo
  wifi.loop();

  // Revisar si llega algo por Serial
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n'); // Lee hasta Enter

    cmd.trim(); // Quita espacios y saltos

    if (cmd.equalsIgnoreCase("RESET")) {
      Serial.println("Comando RESET recibido. Reiniciando WiFi...");
      wifi.reset(); // Borra datos guardados y reinicia
    }
  }
}