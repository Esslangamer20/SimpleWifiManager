#pragma once

#ifdef ESP32
  #include <WiFi.h>
  #include <WebServer.h>
  #include <Preferences.h>
  #include <BluetoothSerial.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#endif

enum DataMode {
  DATA_SERIAL,
  DATA_BLUETOOTH
};

class SimpleWiFiManager {
public:
  SimpleWiFiManager();

  void begin();
  void loop();
  void reset();
  void setDataMode(DataMode mode);

private:
  void startPortal();
  DataMode currentMode = DATA_SERIAL;

#ifdef ESP32
  WebServer server{80};
  BluetoothSerial bt;
#else
  ESP8266WebServer server{80};
#endif

  String deviceName = "ESP32-Setup";
  String savedSSID = "";
  String savedPASS = "";
};
