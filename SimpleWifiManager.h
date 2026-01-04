#ifndef SIMPLE_WIFI_MANAGER_H
#define SIMPLE_WIFI_MANAGER_H

// Librerias basicas de ESP32
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

// Clase principal de la libreria
class SimpleWiFiManager {
  public:
    // Inicia el WiFi:
    // - Si hay datos guardados, intenta conectar
    // - Si falla, crea un portal de configuracion
    void begin();

    // Debe llamarse SIEMPRE dentro del loop()
    // Mantiene funcionando el servidor web
    void loop();

    // Borra las credenciales guardadas
    // y reinicia el ESP32
    void reset();

  private:
    // Servidor web (se usa puntero para evitar errores de copia)
    WebServer* server = nullptr;

    // Preferencias para guardar SSID y password
    Preferences prefs;

    // Variables donde se guarda el WiFi
    String ssid;
    String pass;

    // Inicia el portal web de configuracion
    void startPortal();
};

#endif
