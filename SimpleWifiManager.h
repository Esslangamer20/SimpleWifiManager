#ifndef SIMPLEWIFIMANAGER_H
#define SIMPLEWIFIMANAGER_H

#if defined(ESP32)
  #include <WiFi.h>
  #include <WebServer.h>
  #include <Preferences.h>
  #include "BluetoothSerial.h"
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <EEPROM.h>
#else
  #error "SimpleWiFiManager solo soporta ESP32 y ESP8266"
#endif

enum DataModeType { DATA_SERIAL, DATA_BLUETOOTH };

class SimpleWiFiManager {
public:
    SimpleWiFiManager();

    void begin();
    void loop();
    void reset();
    bool isConnected() { return _connected; }

    void setDataMode(DataModeType mode);

    void sendData(const String &msg); // Mensajes seguros

private:
    const char* _ssid;
    const char* _password;
    bool _connected;

    DataModeType _dataMode;

#if defined(ESP32)
    Preferences prefs;
    WebServer server{80};
    BluetoothSerial BT;
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
