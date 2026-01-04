#ifndef SIMPLEWIFIMANAGER_H
#define SIMPLEWIFIMANAGER_H

#if defined(ESP32)
  #include <WiFi.h>
  #include <Preferences.h> // Guardar WiFi en ESP32
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <EEPROM.h>      // Guardar WiFi en ESP8266
#else
  #error "SimpleWiFiManager solo soporta ESP32 y ESP8266"
#endif

class SimpleWiFiManager {
public:
    SimpleWiFiManager();

    // Inicia WiFi (automático si no se pasan parámetros)
    void begin(const char* ssid = nullptr, const char* password = nullptr);

    // Debe llamarse siempre dentro de loop()
    void loop();

    // Borra WiFi guardado y reinicia
    void reset();

    // Saber si está conectado a WiFi
    bool isConnected() { return _connected; }

private:
#if defined(ESP32)
    Preferences prefs; // Para guardar SSID/Password
#elif defined(ESP8266)
    void saveWiFiEEPROM(const char* ssid, const char* password);
    void loadWiFiEEPROM(char* ssid, char* password);
#endif

    const char* _ssid;
    const char* _password;
    bool _connected;
};

#endif
