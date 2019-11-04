#include "OTA.h"
#include "SerialMarcadores.h"

const int led =  LED_BUILTIN;
boolean estadoLed = false;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  setup_OTA();
}

void loop() {
  revisaOTA();
  buscaMarcadores();
  enciendeLed();
}

void enciendeLed(){
    digitalWrite(led, estadoLed);
}
