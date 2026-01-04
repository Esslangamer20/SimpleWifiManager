#include "SimpleWiFiManager.h"

// Constructor
SimpleWiFiManager::SimpleWiFiManager() {
    _ssid = nullptr;
    _password = nullptr;
    _connected = false;
}

void SimpleWiFiManager::begin(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;

#if defined(ESP32)
    prefs.begin("wifi", false);

    // Cargar guardado si no se pasó SSID
    if (!_ssid || strlen(_ssid) == 0) {
        _ssid = prefs.getString("ssid").c_str();
        _password = prefs.getString("pass").c_str();
    }

    if (!_ssid || strlen(_ssid) == 0) {
        // No hay WiFi → crear AP
        Serial.println("No WiFi guardado, creando Access Point...");
        WiFi.softAP("SimpleWiFiManager");
        Serial.print("IP del AP: ");
        Serial.println(WiFi.softAPIP());
        _connected = false;
    } else {
        WiFi.begin(_ssid, _password);
        Serial.print("Intentando conectarse a WiFi: ");
        Serial.println(_ssid);
    }

#elif defined(ESP8266)
    char savedSSID[32];
    char savedPASS[64];

    if (!_ssid || strlen(_ssid) == 0) {
        loadWiFiEEPROM(savedSSID, savedPASS);
        _ssid = savedSSID;
        _password = savedPASS;
    }

    if (!_ssid || strlen(_ssid) == 0) {
        Serial.println("No WiFi guardado, creando Access Point...");
        WiFi.softAP("SimpleWiFiManager");
        Serial.print("IP del AP: ");
        Serial.println(WiFi.softAPIP());
        _connected = false;
    } else {
        WiFi.begin(_ssid, _password);
        Serial.print("Intentando conectarse a WiFi: ");
        Serial.println(_ssid);
    }
#endif
}

// Mantener conexión y detectar RESET
void SimpleWiFiManager::loop() {
#if defined(ESP32) || defined(ESP8266)
    if (WiFi.status() == WL_CONNECTED) {
        _connected = true;
    } else {
        _connected = false;
    }

    // Imprimir IP solo la primera vez
    static bool printedIP = false;
    if (_connected && !printedIP) {
        Serial.print("Conectado! IP: ");
        Serial.println(WiFi.localIP());
        printedIP = true;
    }

    // RESET por Serial
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.equalsIgnoreCase("RESET")) {
            Serial.println("Comando RESET recibido. Reiniciando...");
            reset();
        }
    }
#endif
}

// Borra WiFi y reinicia
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
#endif    } else {
        _connected = false;
    }

    // Imprimir IP solo la primera vez
    static bool printedIP = false;
    if (_connected && !printedIP) {
        Serial.print("Conectado! IP: ");
        Serial.println(WiFi.localIP());
        printedIP = true;
    }

    // RESET por Serial
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.equalsIgnoreCase("RESET")) {
            Serial.println("Comando RESET recibido. Reiniciando...");
            reset();
        }
    }
#endif
}

// Borra WiFi y reinicia
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
#endif#if defined(ESP8266)
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
