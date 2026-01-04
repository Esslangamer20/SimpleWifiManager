#include "SimpleWiFiManager.h"

// ==========================
// Metodo principal
// ==========================
void SimpleWiFiManager::begin() {
  // Abre el espacio de memoria llamado "wifi"
  prefs.begin("wifi", false);

  // Lee SSID y password guardados
  ssid = prefs.getString("ssid", "");
  pass = prefs.getString("pass", "");

  // Si hay un SSID guardado, intenta conectar
  if (ssid.length() > 0) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());

    // Espera hasta 10 segundos a que conecte
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
      delay(500);
    }

    // Si conecto correctamente
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
      return; // Sale sin abrir portal
    }
  }

  // Si no conecto, abre portal
  startPortal();
}

// ==========================
// Portal de configuracion
// ==========================
void SimpleWiFiManager::startPortal() {
  // Cambia a modo Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32-Setup");

  // Crea el servidor web
  server = new WebServer(80);

  // Pagina principal
  server->on("/", [this]() {
    server->send(200, "text/html",
      "<h2>Configurar WiFi</h2>"
      "<form action='/save'>"
      "SSID:<br><input name='s'><br><br>"
      "Password:<br><input name='p' type='password'><br><br>"
      "<input type='submit' value='Guardar'>"
      "</form>"
    );
  });

  // Guardar datos
  server->on("/save", [this]() {
    ssid = server->arg("s");
    pass = server->arg("p");

    // Guarda en memoria flash
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);

    server->send(200, "text/html", "Guardado. Reiniciando...");
    delay(1500);
    ESP.restart();
  });

  // Inicia servidor
  server->begin();
}

// ==========================
// Mantiene el servidor vivo
// ==========================
void SimpleWiFiManager::loop() {
  // Atiende peticiones web si el portal esta activo
  if (server) {
    server->handleClient();
  }
}

// ==========================
// Borra WiFi guardado
// ==========================
void SimpleWiFiManager::reset() {
  // Borra preferencias
  prefs.begin("wifi", false);
  prefs.clear();
  prefs.end();

  // Reinicia el ESP32
  ESP.restart();
}}
