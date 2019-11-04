#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <WiFi101OTA.h>
#include "arduino_secrets.h"

extern int      PinBotonGrande;
extern int      PinLedFinBotonGrande;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void setup_OTA() {
  if (WiFi.status() == WL_NO_SHIELD) {
    while (true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  
  WiFiOTA.begin("", "", InternalStorage);

  printWifiStatus();
}

void revisaOTA(){
  WiFiOTA.poll();
}





