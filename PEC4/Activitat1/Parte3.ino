const int led = LED_BUILTIN;
const int pinSalida = 7;
const int pinLectura = 0;
const long intervaloTiempo = 1000;
volatile byte state = LOW;

int estadoPin = LOW;
unsigned long tiempoPrevio = 0;
unsigned long tiempoActual = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(pinSalida, OUTPUT);
  pinMode(pinLectura, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinLectura), escribeLed, CHANGE);
}

void loop() {
  tiempoActual = millis();
  procesaTiempos();
  digitalWrite(led, state);
}

void procesaTiempos(){
  if (tiempoActual - tiempoPrevio >= intervaloTiempo) {
    tiempoPrevio = tiempoActual;
    cambiaEstado();
    escribePin();
  }
}

void cambiaEstado(){
  if (estadoPin == LOW) {
      estadoPin = HIGH;
    } else {
      estadoPin = LOW;
    }
}

void escribePin(){
  digitalWrite(pinSalida, estadoPin);
}

void escribeLed(){
  state = !state;

}
