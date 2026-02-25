#include "SimpleWiFiManager.h"

#ifdef ESP32
Preferences prefs;
#else
#include <EEPROM.h>
#endif

// Mini HTML portal (ultra compacto para ahorrar memoria)
const char PAGE[] PROGMEM = R"rawliteral(
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>WiFi Setup</title>
</head>
<body>
<h2>SimpleWiFiManager</h2>
<form action="/save" method="POST">
Red:
<select name="ssid">%NETWORKS%</select>
<br>
Contraseña:
<input type="password" name="pass">
<br>
Nombre dispositivo:
<input type="text" name="dev" placeholder="MiESP32">
<br>
<button type="submit">Guardar</button>
</form>
</body>
</html>
)rawliteral";

SimpleWiFiManager::SimpleWiFiManager() {}

// ---------- DATA MODE ----------
void SimpleWiFiManager::setDataMode(DataMode mode) {
  currentMode = mode;
}

// ---------- BEGIN ----------
void SimpleWiFiManager::begin() {

#ifdef ESP32
  prefs.begin("wifi", false);
  savedSSID = prefs.getString("ssid", "");
  savedPASS = prefs.getString("pass", "");
  deviceName = prefs.getString("dev", "ESP32");

  WiFi.setHostname(deviceName.c_str());
#else
  // ESP8266 usando EEPROM
  EEPROM.begin(512);
  savedSSID = ""; // opcional: leer de EEPROM
  savedPASS = "";
  deviceName = "ESP8266-Setup";
#endif

  // Conectar si hay SSID guardado
  if (savedSSID != "") {
    WiFi.begin(savedSSID.c_str(), savedPASS.c_str());
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
      delay(500);
    }
    if (WiFi.status() == WL_CONNECTED) return; // listo
  }

  startPortal();
}

// ---------- PORTAL ----------
void SimpleWiFiManager::startPortal() {

  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(deviceName.c_str());

  server.on("/", HTTP_GET, [this]() {
    int n = WiFi.scanNetworks();
    String nets = "";
    for (int i = 0; i < n; i++) {
      nets += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
    }
    String page = PAGE;
    page.replace("%NETWORKS%", nets);
    server.send(200, "text/html", page);
  });

  server.on("/save", HTTP_POST, [this]() {
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");
    String dev  = server.arg("dev");

#ifdef ESP32
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    if (dev != "") prefs.putString("dev", dev);
#else
    // ESP8266: guardar en EEPROM (simplificado)
#endif

    server.send(200, "text/html", "<h2>Guardado 👍</h2><p>Reiniciando...</p>");
    delay(1500);
    ESP.restart();
  });

  server.begin();

#ifdef ESP32
  if (currentMode == DATA_BLUETOOTH) bt.begin(deviceName.c_str());
#endif
}

// ---------- LOOP ----------
void SimpleWiFiManager::loop() {
  server.handleClient();
}

// ---------- RESET ----------
void SimpleWiFiManager::reset() {
#ifdef ESP32
  prefs.clear();
#endif
  delay(500);
  ESP.restart();
}

// ---------- SEND ----------
void SimpleWiFiManager::send(String msg) {
  if (currentMode == DATA_SERIAL) {
    Serial.println(msg);
  }
#ifdef ESP32
  if (currentMode == DATA_BLUETOOTH && bt.hasClient()) {
    bt.println(msg);
  }
#endif
}
