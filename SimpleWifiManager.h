#ifndef SIMPLEWIFIMANAGER_H
#define SIMPLEWIFIMANAGER_H

// Detecta la plataforma automáticamente
#if defined(ESP32)
  #include <WiFi.h>
  #include <Preferences.h> // Para guardar WiFi en ESP32
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <EEPROM.h> // Para guardar WiFi en ESP8266
#else
  #error "SimpleWiFiManager solo soporta ESP32 y ESP8266"
#endif

class SimpleWiFiManager {
public:
    SimpleWiFiManager();

    // Inicializa el WiFi
    void begin(const char* ssid = nullptr, const char* password = nullptr);

    // Debe llamarse siempre dentro de loop()
    void loop();

    // Borra los datos guardados y reinicia
    void reset();

private:
#if defined(ESP32)
    Preferences prefs; // Guarda SSID y contraseña
#elif defined(ESP8266)
    void saveWiFiEEPROM(const char* ssid, const char* password);
    void loadWiFiEEPROM(char* ssid, char* password);
#endif

    const char* _ssid;
    const char* _password;
    bool _connected;
};

#endif