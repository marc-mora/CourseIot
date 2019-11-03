const int led =  LED_BUILTIN;
const long intervaloTiempo = 1000;

int estadoLed = LOW;
unsigned long tiempoPrevio = 0;
unsigned long tiempoActual = 0;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  tiempoActual = millis();
  procesaTiempos();
}

void procesaTiempos(){
  if (tiempoActual - tiempoPrevio >= intervaloTiempo) {
    tiempoPrevio = tiempoActual;
    cambiaEstado();
    escribePin();
  }
}

void cambiaEstado(){
  if (estadoLed == LOW) {
      estadoLed = HIGH;
    } else {
      estadoLed = LOW;
    }
}

void escribePin(){
  digitalWrite(led, estadoLed);
}

