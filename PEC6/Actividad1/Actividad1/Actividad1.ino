#include <Arduino_MKRENV.h>
#include <SPI.h>
#include <SD.h>

const int SD_CS_PIN = 4;  
const unsigned long millisUnDia = 1000*60*60*24;
const String fichero = "sensores24h.csv";

unsigned long tiempoTranscurrido = 0;
unsigned long   initTime = 0;
unsigned long   initInTime = 0;
float           temperature = 0;
float           humidity = 0;
float           pressure = 0;
float           UVA = 0;
float           UVB = 0;
float           UVIndex = 0;

File dataFile;

void setup() {
  initTime = millis();
  Serial.begin(9600);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }

  SPI.begin();
  delay(100);

  if(!SD.begin(SD_CS_PIN)) {
    Serial.println("Failed to initialize SD card!");
    while (1);
  }

  abrirFichero();
  crearColumnas();
  cerrarFichero();
  delay(100);
}

void loop() {
  verificarDia();
  abrirFichero();
  leerSensores();
  guardarDatos();
  //enviarSerial();
  cerrarFichero();
  delay(1000);
}

void verificarDia(){
  initTime = millis();
  tiempoTranscurrido = initTime - initInTime;
  Serial.println(tiempoTranscurrido);
  if (tiempoTranscurrido >= millisUnDia) {
    borraFichero();
    initInTime += millisUnDia;
  }
}

void crearColumnas(){
  dataFile.println("temperature,humidity,pressure,UVA,UVB,UVindex");
}


void abrirFichero(){
  dataFile = SD.open(fichero, FILE_WRITE);
  delay(1000);
}

void cerrarFichero(){
  dataFile.close();
}

void borraFichero(){
  SD.remove(fichero);
  Serial.println("Fichero borrado: ");
}

void leerSensores(){
  temperature = ENV.readTemperature();
  humidity = ENV.readHumidity();
  pressure = ENV.readPressure();
  UVA = ENV.readUVA();
  UVB = ENV.readUVB();
  UVIndex = ENV.readUVIndex();
}

void guardarDatos(){
  dataFile.print(temperature);
  dataFile.print(",");
  dataFile.print(humidity);
  dataFile.print(",");
  dataFile.print(pressure);
  dataFile.print(",");
  dataFile.print(UVA);
  dataFile.print(",");
  dataFile.print(UVB);
  dataFile.print(",");
  dataFile.println(UVIndex);
}

void enviarSerial(){
  Serial.print("temperature: ");
  Serial.println(temperature);
  Serial.print("humidity: ");
  Serial.println(humidity);
  Serial.print("pressure: ");
  Serial.println(pressure);
  Serial.print("UVA: ");
  Serial.println(UVA);
  Serial.print("UVB: ");
  Serial.println(UVB);
  Serial.print("UVIndex: ");
  Serial.println(UVIndex);

}
