#ifndef SIMPLEWIFIMANAGER_H
#define SIMPLEWIFIMANAGER_H

#if defined(ESP32)
  #include <WiFi.h>
  #include <WebServer.h>
  #include <Preferences.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <EEPROM.h>
#else
  #error "SimpleWiFiManager solo soporta ESP32 y ESP8266"
#endif

class SimpleWiFiManager {
public:
    SimpleWiFiManager();

    void begin();
    void loop();
    void reset();
    bool isConnected() { return _connected; }

private:
    const char* _ssid;
    const char* _password;
    bool _connected;

#if defined(ESP32)
    Preferences prefs;
    WebServer server{80};
#elif defined(ESP8266)
    WebServer server{80};
    void saveWiFiEEPROM(const char* ssid, const char* password);
    void loadWiFiEEPROM(char* ssid, char* password);
#endif

    void startAP();
    void startWebServer();
    void handleRoot();
    void handleConnect();
};

#endif