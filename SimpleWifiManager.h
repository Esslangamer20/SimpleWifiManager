#ifndef SIMPLE_WIFI_MANAGER_H
#define SIMPLE_WIFI_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

class SimpleWiFiManager {
  public:
    // Inicia el WiFi o el portal de configuracion
    void begin();

    // Debe llamarse dentro de loop()
    void loop();

    // Borra las credenciales guardadas
    void reset();

  private:
    WebServer server = WebServer(80);
    Preferences prefs;

    String ssid;
    String pass;

    void startPortal();
};

#endif