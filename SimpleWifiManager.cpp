#include "SimpleWiFiManager.h"

/* ========= CONSTRUCTOR ========= */

SimpleWiFiManager::SimpleWiFiManager() {
  _ssid = nullptr;
  _password = nullptr;
  _connected = false;
  _dataMode = DATA_SERIAL;
}

/* ========= DATA MODE ========= */

void SimpleWiFiManager::setDataMode(DataModeType mode) {
#if defined(ESP8266)
  if (mode == DATA_BLUETOOTH) {
    Serial.println("ESP8266 no tiene Bluetooth, usando Serial");
    _dataMode = DATA_SERIAL;
    return;
  }
#endif
  _dataMode = mode;
}

void SimpleWiFiManager::sendData(const String &msg) {
  if (_dataMode == DATA_SERIAL) {
    Serial.println(msg);
  }
#if defined(ESP32)
  else if (_dataMode == DATA_BLUETOOTH) {
    BT.println(msg);
  }
#endif
}

/* ========= BEGIN ========= */

void SimpleWiFiManager::begin() {

#if defined(ESP32)
  if (_dataMode == DATA_BLUETOOTH) {
    BT.begin("ESP32_BT");
    sendData("Bluetooth iniciado");
  }
#endif

#if defined(ESP32)
  prefs.begin("wifi", false);
  String s = prefs.getString("ssid", "");
  String p = prefs.getString("pass", "");
  _ssid = s.length() ? strdup(s.c_str()) : nullptr;
  _password = p.length() ? strdup(p.c_str()) : nullptr;
#elif defined(ESP8266)
  static char ssid[32], pass[64];
  loadWiFiEEPROM(ssid, pass);
  _ssid = strlen(ssid) ? ssid : nullptr;
  _password = strlen(pass) ? pass : nullptr;
#endif

  if (!_ssid) {
    startAP();
    startWebServer();
  } else {
    sendData("Conectando a WiFi: " + String(_ssid));
    WiFi.begin(_ssid, _password);
  }
}

/* ========= LOOP ========= */

void SimpleWiFiManager::loop() {

  if (WiFi.status() == WL_CONNECTED) {
    if (!_connected) {
      _connected = true;
      sendData("Conectado! IP: " + WiFi.localIP().toString());
    }
  } else {
    _connected = false;
  }

  // RESET por Serial
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.equalsIgnoreCase("RESET")) {
      sendData("RESET recibido por Serial");
      reset();
    }
  }

#if defined(ESP32)
  // RESET por Bluetooth
  if (_dataMode == DATA_BLUETOOTH && BT.available()) {
    String cmd = BT.readStringUntil('\n');
    cmd.trim();
    if (cmd.equalsIgnoreCase("RESET")) {
      sendData("RESET recibido por Bluetooth");
      reset();
    }
  }
#endif

  server.handleClient();
}

/* ========= RESET ========= */

void SimpleWiFiManager::reset() {
#if defined(ESP32)
  prefs.clear();
#elif defined(ESP8266)
  saveWiFiEEPROM("", "");
#endif
  delay(500);
  ESP.restart();
}

bool SimpleWiFiManager::isConnected() {
  return _connected;
}

/* ========= AP + WEB ========= */

void SimpleWiFiManager::startAP() {
  WiFi.softAP("SimpleWiFiManager");
  sendData("AP creado: SimpleWiFiManager");
  sendData("IP AP: " + WiFi.softAPIP().toString());
}

void SimpleWiFiManager::startWebServer() {
  server.on("/", [&]() { handleRoot(); });
  server.on("/connect", [&]() { handleConnect(); });
  server.begin();
  sendData("Portal web activo");
}

void SimpleWiFiManager::handleRoot() {
  String html = "<h1>Configurar WiFi</h1>";
  html += "<form action='/connect' method='POST'>";
  html += "SSID:<select name='ssid'>";

  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    html += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
  }

  html += "</select><br>";
  html += "Password:<input type='password' name='pass'><br>";
  html += "<input type='submit' value='Conectar'></form>";

  server.send(200, "text/html", html);
}

void SimpleWiFiManager::handleConnect() {
  if (!server.hasArg("ssid") || !server.hasArg("pass")) {
    server.send(400, "text/html", "Faltan datos");
    return;
  }

  _ssid = strdup(server.arg("ssid").c_str());
  _password = strdup(server.arg("pass").c_str());

  sendData("Conectando a WiFi: " + String(_ssid));

  String stars = "";
  for (int i = 0; i < strlen(_password); i++) stars += "*";
  sendData("Password: " + stars);

#if defined(ESP32)
  prefs.putString("ssid", _ssid);
  prefs.putString("pass", _password);
#elif defined(ESP8266)
  saveWiFiEEPROM(_ssid, _password);
#endif

  WiFi.begin(_ssid, _password);
  server.send(200, "text/html", "<h1>Conectando...</h1>");
}

/* ========= EEPROM ESP8266 ========= */

#if defined(ESP8266)
void SimpleWiFiManager::saveWiFiEEPROM(const char* ssid, const char* pass) {
  EEPROM.begin(512);
  for (int i = 0; i < 32; i++) EEPROM.write(i, ssid[i]);
  for (int i = 0; i < 64; i++) EEPROM.write(32 + i, pass[i]);
  EEPROM.commit();
}

void SimpleWiFiManager::loadWiFiEEPROM(char* ssid, char* pass) {
  EEPROM.begin(512);
  for (int i = 0; i < 32; i++) ssid[i] = EEPROM.read(i);
  for (int i = 0; i < 64; i++) pass[i] = EEPROM.read(32 + i);
}
#endif