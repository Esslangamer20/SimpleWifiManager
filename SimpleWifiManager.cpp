#include "SimpleWiFiManager.h"

SimpleWiFiManager::SimpleWiFiManager() {
    _ssid = nullptr;
    _password = nullptr;
    _connected = false;
}

void SimpleWiFiManager::begin() {
#if defined(ESP32)
    prefs.begin("wifi", false);
    _ssid = prefs.getString("ssid").c_str();
    _password = prefs.getString("pass").c_str();
#elif defined(ESP8266)
    char savedSSID[32];
    char savedPASS[64];
    loadWiFiEEPROM(savedSSID, savedPASS);
    _ssid = savedSSID;
    _password = savedPASS;
#endif

    if (!_ssid || strlen(_ssid) == 0) {
        // No hay WiFi guardado → crear AP
        startAP();
        startWebServer();
    } else {
        // Conectar a WiFi guardado
        WiFi.begin(_ssid, _password);
        Serial.print("Conectando a WiFi: "); Serial.println(_ssid);
    }
}

void SimpleWiFiManager::loop() {
#if defined(ESP32) || defined(ESP8266)
    if (WiFi.status() == WL_CONNECTED) {
        _connected = true;
    } else {
        _connected = false;
    }

    static bool printedIP = false;
    if (_connected && !printedIP) {
        Serial.print("Conectado! IP: "); Serial.println(WiFi.localIP());
        printedIP = true;
    }

    // Reset por Serial
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n'); cmd.trim();
        if (cmd.equalsIgnoreCase("RESET")) {
            Serial.println("RESET recibido. Reiniciando...");
            reset();
        }
    }

    // Manejar clientes web
    server.handleClient();
#endif
}

void SimpleWiFiManager::reset() {
#if defined(ESP32)
    prefs.clear();
#elif defined(ESP8266)
    saveWiFiEEPROM("", "");
#endif
    _ssid = nullptr;
    _password = nullptr;
    ESP.restart();
}

void SimpleWiFiManager::startAP() {
    Serial.println("Creando Access Point...");
    WiFi.softAP("SimpleWiFiManager");
    Serial.print("IP AP: "); Serial.println(WiFi.softAPIP());
}

void SimpleWiFiManager::startWebServer() {
    server.on("/", [&]{ handleRoot(); });
    server.on("/connect", [&]{ handleConnect(); });
    server.begin();
    Serial.println("Servidor web iniciado en AP");
}

// Muestra la página principal con SSID detectadas
void SimpleWiFiManager::handleRoot() {
    String page = "<h1>Configura tu WiFi</h1>";
    page += "<form action='/connect' method='POST'>";
    page += "SSID:<select name='ssid'>";

    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++) {
        String ssid = WiFi.SSID(i);
        page += "<option value='" + ssid + "'>" + ssid + "</option>";
    }
    page += "</select><br>";
    page += "Password:<input type='password' name='pass'><br>";
    page += "<input type='submit' value='Conectar'></form>";
    server.send(200, "text/html", page);
}

// Procesa el formulario y conecta
void SimpleWiFiManager::handleConnect() {
    if (server.hasArg("ssid") && server.hasArg("pass")) {
        _ssid = server.arg("ssid").c_str();
        _password = server.arg("pass").c_str();

        Serial.print("Conectando a: "); Serial.println(_ssid);
        WiFi.begin(_ssid, _password);

#if defined(ESP32)
        prefs.putString("ssid", _ssid);
        prefs.putString("pass", _password);
#elif defined(ESP8266)
        saveWiFiEEPROM(_ssid, _password);
#endif
        server.send(200, "text/html", "<h1>Intentando conectar...</h1>");
    } else {
        server.send(200, "text/html", "<h1>Error: Faltan datos</h1>");
    }
}

#if defined(ESP8266)
void SimpleWiFiManager::saveWiFiEEPROM(const char* ssid, const char* password) {
    EEPROM.begin(512);
    for (int i = 0; i < 32; i++) EEPROM.write(i, ssid[i]);
    for (int i = 0; i < 64; i++) EEPROM.write(32 + i, password[i]);
    EEPROM.commit();
}

void SimpleWiFiManager::loadWiFiEEPROM(char* ssid, char* password) {
    EEPROM.begin(512);
    for (int i = 0; i < 32; i++) ssid[i] = EEPROM.read(i);
    for (int i = 0; i < 64; i++) password[i] = EEPROM.read(32 + i);
}
#endif