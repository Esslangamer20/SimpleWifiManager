# SimpleWiFiManager

Librería ligera para crear un portal WiFi de configuración en ESP32,
pensada para funcionar sin problemas en ArduinoDroid.

## Uso básico

#include <SimpleWiFiManager.h>

SimpleWiFiManager wifi;

void setup() {
  Serial.begin(115200);
  wifi.begin();
}

void loop() {
  wifi.loop();
}

## Características

- Portal WiFi simple (AP + Web)
- Escaneo de redes WiFi cercanas
- DataMode: salida por Serial o Bluetooth (ESP32)
- No imprime contraseñas (seguridad)
- Compatible con ArduinoDroid
- Fácil de usar y ligera

## Portal WiFi

Conéctate a la red:

ESP32-Setup

Luego abre en el navegador:

http://192.168.4.1

## Comandos

RESET
- Reinicia el ESP32.
- La conexión Bluetooth se pierde y debe reconectarse manualmente.

## Inspiración

Al intentar usar WiFiManager en ArduinoDroid aparecieron muchos errores
de compatibilidad.

Por eso nació SimpleWiFiManager: una alternativa simple,
funcional y sin complicaciones 👍

## Plataformas soportadas

- ESP32 (Serial + Bluetooth)
- ESP8266 (solo Serial, sin Bluetooth)
