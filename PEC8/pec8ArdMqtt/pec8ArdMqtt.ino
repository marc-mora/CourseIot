/*
 This example connects to an unencrypted WiFi network.
 Then it prints the  MAC address of the WiFi shield,
 the IP address obtained, and other network details.
 Circuit:
 * WiFi shield attached
 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */
#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include <Arduino_MKRENV.h>
#include <ArduinoJson.h>
#include <string.h>

#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)
char mqttuser[] = MQTT_USER;      // User
char mqttpass[] = MQTT_PASS;      // Password
int status = WL_IDLE_STATUS;     // the WiFi radio's status
//IPAddress mqttserver(192, 168, 43, 181);
IPAddress mqttserver(54, 154, 59, 72);
const int mqttport = 1883;
//const int mqttport = 8883;
int LedPin = 9;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

// Initialize the client library
WiFiSSLClient wifiClient;
PubSubClient mqttClient(mqttserver, mqttport, callback, wifiClient);


long lastReconnectAttempt = 0;
long now;
// Callback function

void subscribeReceive(char* topic, byte* payload, unsigned int length)
{
  String answer;
  // Print the topic
  Serial.print("Topic: ");
  Serial.println(topic);
 
  // Print the message
  Serial.print("Message: ");
  for(int i = 0; i < length; i ++)
  {
    //Serial.print(char(payload[i]));
    answer = String(answer + char(payload[i]));
    
  }
  //
  StaticJsonDocument<300> doc;
   DeserializationError error = deserializeJson(doc, answer);
   //String set = doc["set"];
    String set = "set";
  
  blink(String (set));
 Serial.println(set);
  // Print a newline
  Serial.println("");
}

void blink(String  set){
      if(set=="0") {
      digitalWrite(LedPin, LOW);
    }else{
      digitalWrite(LedPin, HIGH);
    } 
  
}


void callback(char* topic, byte* payload, unsigned int length) {
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.

  // Allocate the correct amount of memory for the payload copy
Serial.println("aa");
  byte* p = (byte*)malloc(length);
  // Copy the payload to the new buffer
  memcpy(p,payload,length);
  mqttClient.publish(topic, p, length);
  // Free the memory
  free(p);
}

boolean reconnect() {
  Serial.print("Attempting MQTT connection...");
  if (mqttClient.connect("192.168.1.142", mqttuser, mqttpass)) {
    Serial.println("Arduino connected to MQTT");
    // Once connected, publish an announcement...
    mqttClient.publish("outTopic","hello world");
    // ... and resubscribe
     mqttClient.subscribe("homie/arduino/mkrenv/relays/#");
  }else {
    Serial.print("Failed, rc=");
    Serial.print(mqttClient.state());
    Serial.println(" try again in 5 seconds");
  }

  return mqttClient.connected();
}



int incomingByte = 0; // for incoming serial data
String input = "";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
   pinMode(LedPin, OUTPUT); //led
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  //Initialize MRKENV
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }  

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWiFiData();

  Serial.print("Attempting MQTT connection...");
  Serial.print(mqttClient.state());
  if (mqttClient.connect("192.168.1.142", mqttuser, mqttpass)) {
    Serial.println("connected");
    //mqttClient.publish("outTopic","hello world");
    mqttClient.subscribe("homie/arduino/mkrenv/relays/#");
    mqttClient.setCallback(subscribeReceive);
    mqttClientPublishInit();
  }
  lastReconnectAttempt = 0;
}

void mqttClientPublishInit(){
  mqttClient.publish("homie/arduino/$homie", "3.0", true);
  mqttClient.publish("homie/arduino/$name", "Arduino MKR1000", true);
  mqttClient.publish("homie/arduino/$nodes", "mkrenv", true);
  mqttClient.publish("homie/arduino/$state", "ready", true);
  mqttClient.publish("homie/arduino/mkrenv/$name", "Grupo4-01", true);
  mqttClient.publish("homie/arduino/mkrenv/$properties", "(temperature, humidity, pressure, illuminance, uva, uvb, uvindex)", true);
  //Temperature
  mqttClient.publish("homie/arduino/mkrenv/temperature/$name", "Temperature", true);
  mqttClient.publish("homie/arduino/mkrenv/temperature/$unit", "ºC", true);
  mqttClient.publish("homie/arduino/mkrenv/temperature/$datatype", "float", true);
  mqttClient.publish("homie/arduino/mkrenv/temperature/$settable", "false", true);
  //Humidity
  mqttClient.publish("homie/arduino/mkrenv/humidity/$name", "Humidity", true);
  mqttClient.publish("homie/arduino/mkrenv/humidity/$unit", "%", true);
  mqttClient.publish("homie/arduino/mkrenv/humidity/$datatype", "float", true);
  mqttClient.publish("homie/arduino/mkrenv/humidity/$settable", "false", true);  
  //Pressure
  mqttClient.publish("homie/arduino/mkrenv/pressure/$name", "Pressure", true);
  mqttClient.publish("homie/arduino/mkrenv/pressure/$unit", "kPa", true);
  mqttClient.publish("homie/arduino/mkrenv/pressure/$datatype", "float", true);
  mqttClient.publish("homie/arduino/mkrenv/pressure/$settable", "false", true); 
  //Illuminance
  mqttClient.publish("homie/arduino/mkrenv/illuminance/$name", "Illuminance", true);
  mqttClient.publish("homie/arduino/mkrenv/illuminance/$unit", "lx", true);
  mqttClient.publish("homie/arduino/mkrenv/illuminance/$datatype", "float", true);
  mqttClient.publish("homie/arduino/mkrenv/illuminance/$settable", "false", true); 
  //UVA
  mqttClient.publish("homie/arduino/mkrenv/uva/$name", "UVA", true);
  mqttClient.publish("homie/arduino/mkrenv/uva/$unit", "", true);
  mqttClient.publish("homie/arduino/mkrenv/uva/$datatype", "float", true);
  mqttClient.publish("homie/arduino/mkrenv/uva/$settable", "false", true);   
  //UVB
  mqttClient.publish("homie/arduino/mkrenv/uvb/$name", "UVB", true);
  mqttClient.publish("homie/arduino/mkrenv/uvb/$unit", "", true);
  mqttClient.publish("homie/arduino/mkrenv/uvb/$datatype", "float", true);
  mqttClient.publish("homie/arduino/mkrenv/uvb/$settable", "false", true);   
  //UVIndex
  mqttClient.publish("homie/arduino/mkrenv/uvindex/$name", "UV Index", true);
  mqttClient.publish("homie/arduino/mkrenv/uvindex/$unit", "", true);
  mqttClient.publish("homie/arduino/mkrenv/uvindex/$datatype", "float", true);
  mqttClient.publish("homie/arduino/mkrenv/uvindex/$settable", "false", true);  
}

void loop()
{
  now = millis();
  if (now - lastReconnectAttempt > 5000) {
    lastReconnectAttempt = now;
    if (!mqttClient.connected()) {
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    } else {
    // Client connected
      mqttClient.loop();
     
      mqttPublish();
    }
  }
}

void mqttPublish(){
   // read all the sensor values
  float temperature = ENV.readTemperature();
  float humidity    = ENV.readHumidity();
  float pressure    = ENV.readPressure();
  float illuminance = ENV.readIlluminance();
  float uva         = ENV.readUVA();
  float uvb         = ENV.readUVB();
  float uvIndex     = ENV.readUVIndex();
  
  // print each of the sensor values and publish its value in mqtt
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");
  //dtostrf(temperature,0, 0, buffer);
  mqttClient.publish("homie/arduino/mkrenv/temperature", String(temperature).c_str(), true);  

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");
  mqttClient.publish("homie/arduino/mkrenv/humidity", String(humidity).c_str(), true);

  Serial.print("Pressure    = ");
  Serial.print(pressure);
  Serial.println(" kPa");
  mqttClient.publish("homie/arduino/mkrenv/pressure", String(pressure).c_str(), true);

  Serial.print("Illuminance = ");
  Serial.print(illuminance);
  Serial.println(" lx");
  mqttClient.publish("homie/arduino/mkrenv/illuminance", String(illuminance).c_str(), true);

  Serial.print("UVA         = ");
  Serial.println(uva);
  mqttClient.publish("homie/arduino/mkrenv/uva", String(uva).c_str(), true);

  Serial.print("UVB         = ");
  Serial.println(uvb);
  mqttClient.publish("homie/arduino/mkrenv/uvb", String(uvb).c_str(), true);

  Serial.print("UV Index    = ");
  Serial.println(uvIndex);
  mqttClient.publish("homie/arduino/mkrenv/uvindex", String(uvIndex).c_str(), true);

  // print an empty line
  Serial.println();

}


void printWiFiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
