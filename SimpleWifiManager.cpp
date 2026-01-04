#include "SimpleWiFiManager.h"

void SimpleWiFiManager::begin() {
  prefs.begin("wifi", false);

  ssid = prefs.getString("ssid", "");
  pass = prefs.getString("pass", "");

  // Intentar conectar si hay WiFi guardado
  if (ssid.length() > 0) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
      delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
      return;
    }
  }

  // Si falla, abrir portal
  startPortal();
}

void SimpleWiFiManager::startPortal() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32-Setup");

  server.on("/", [this]() {
    server.send(
      200,
      "text/html",
      "<h2>Configurar WiFi</h2>"
      "<form action='/save'>"
      "Red WiFi:<br>"
      "<input name='s'><br><br>"
      "Contraseña:<br>"
      "<input name='p' type='password'><br><br>"
      "<input type='submit' value='Guardar'>"
      "</form>"
    );
  });

  server.on("/save", [this]() {
    ssid = server.arg("s");
    pass = server.arg("p");

    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);

    server.send(
      200,
      "text/html",
      "Guardado. Reiniciando..."
    );

    delay(1500);
    ESP.restart();
  });

  server.begin();
}

void SimpleWiFiManager::loop() {
  server.handleClient();
}

void SimpleWiFiManager::reset() {
  prefs.clear();
}