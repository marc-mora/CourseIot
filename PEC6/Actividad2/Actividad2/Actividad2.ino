#include <Arduino_MKRENV.h>

const char* ssid = "";
const char* pass =  "";
const char* mqttServer = "10.12.1.55";
const int   mqttPort = 1883;
const char* deviceId = "arduino-1";
const char* userBroker = "curso_iot";
const char* passBroker = "";
const char* callbackTopic = "homie/arduino-1/callback/#";
char* topic = "";

float           temperature = 0;
float           humidity = 0;
float           pressure = 0;
float           UVA = 0;
float           UVB = 0;
float           UVIndex = 0;

void setup() {
  delay(9000);
  Serial.begin(9600);
  Serial.println("init");
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }
  
  setup_MQTT();
  delay(100);
}

void loop() { 
  leerSensores();
  loop_MQTT();
  publicarDatos();
  delay(9000);
}

void leerSensores(){
  temperature = ENV.readTemperature();
  humidity = ENV.readHumidity();
  pressure = ENV.readPressure();
  UVA = ENV.readUVA();
  UVB = ENV.readUVB();
  UVIndex = ENV.readUVIndex();
}

void publicarDatos(){
  topic = "homie/arduino-1/sensores/temperatura";
  publicar("temperatura", String(temperature), topic);
  topic = "homie/arduino-1/sensores/humedad";
  publicar("humedad", String(humidity), topic);
  topic = "homie/arduino-1/sensores/presion";
  publicar("presion", String(pressure), topic);
  topic = "homie/arduino-1/sensores/uva";
  publicar("uva", String(UVA), topic);
  topic = "homie/arduino-1/sensores/uvb";
  publicar("uvb", String(UVB), topic);
  topic = "homie/arduino-1/sensores/indice-uv";
  publicar("indice-uv", String(UVIndex), topic);
}
