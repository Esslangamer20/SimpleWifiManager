#include "SimpleWiFiManager.h"

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

    // Si no pasaron SSID, cargar guardado
    if (!_ssid) {
        _ssid = prefs.getString("ssid").c_str();
        _password = prefs.getString("pass").c_str();
    }

    WiFi.begin(_ssid, _password);

#elif defined(ESP8266)
    char savedSSID[32];
    char savedPASS[64];

    // Si no pasaron SSID, cargar guardado
    if (!_ssid) {
        loadWiFiEEPROM(savedSSID, savedPASS);
        _ssid = savedSSID;
        _password = savedPASS;
    }

    WiFi.begin(_ssid, _password);
#endif
}

void SimpleWiFiManager::loop() {
    // Mantener WiFi conectado
#if defined(ESP32)
    if (WiFi.status() == WL_CONNECTED) {
        _connected = true;
    } else {
        _connected = false;
    }
#elif defined(ESP8266)
    if (WiFi.status() == WL_CONNECTED) {
        _connected = true;
    } else {
        _connected = false;
    }
#endif
}

void SimpleWiFiManager::reset() {
#if defined(ESP32)
    prefs.clear();
#elif defined(ESP8266)
    saveWiFiEEPROM("", "");
#endif
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
#endif