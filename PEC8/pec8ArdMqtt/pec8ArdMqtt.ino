
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
char mqttpass[] = MQTT_PASS;
char mqtthost[] = MQTT_HOST;
// Password
int status = WL_IDLE_STATUS;     // the WiFi radio's status
IPAddress mqttserver(54, 154, 59, 72);
const int mqttport = 1883;
const int relay1 = 1;
const int relay2 = 2;
boolean rele1State = LOW;
boolean rele2State = LOW;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

// Initialize the client library
WiFiSSLClient wifiClient;
PubSubClient mqttClient(mqttserver, mqttport, callback, wifiClient);

long lastReconnectAttempt = 0;
long now;

void callback(char* topic, byte* payload, unsigned int length) {
  String topicStr = topic;
  Serial.println(topicStr);
  Serial.println(payload[0]);
  //homie/arduino/relays/relays/rele2/set
  //49
  //homie/arduino/relays/relays/rele1/set
  //48
  //acciones segun payload y publicar confirmacion del mensaje
  if (topicStr == "homie/arduino/relays/relays/rele1/set"){
    if (payload[0] == '0') {
      rele1State = LOW;
    }
    if (payload[0] == '1') {
      rele1State = HIGH;
    }
  }
  
  if (topicStr == "homie/arduino/relays/relays/rele2/set"){
    if (payload[0] == '0') {
      rele2State = LOW;
    }
    if (payload[0] == '1') {
      rele2State = HIGH;
    }
  }
}

boolean reconnect() {

  if (mqttClient.connect(mqtthost, mqttuser, mqttpass)) {
    mqttClient.publish("outTopic","hello world");
    mqttClient.subscribe("homie/arduino/relays/+/set");
  }

  return mqttClient.connected();
}



int incomingByte = 0; // for incoming serial data
String input = "";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, rele1State);
  digitalWrite(relay2, rele2State);

  //Initialize MRKENV
  if (!ENV.begin()) {
    while (1);
  }  

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  if (mqttClient.connect(mqtthost, mqttuser, mqttpass)) {
    mqttClient.subscribe("homie/arduino/relays/relays/+/set");
    mqttClientPublishInit();
  }
  lastReconnectAttempt = 0;
}

void mqttClientPublishInit(){
  mqttClient.publish("homie/arduino/$homie", "3.0", true);
  mqttClient.publish("homie/arduino/$name", "Arduino MKR1000", true);
  mqttClient.publish("homie/arduino/$nodes", "(mkrenv,relays)", true);
  mqttClient.publish("homie/arduino/$state", "ready", true);
  mqttClient.publish("homie/arduino/mkrenv/$name", "Grupo4-env-01", true);
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
  //Relays
  mqttClient.publish("homie/arduino/relays/$name", "Grupo4-relays-01", true);
  mqttClient.publish("homie/arduino/relays/$properties", "(rele1, rele2)", true);  
    //Relay1
  mqttClient.publish("homie/arduino/relays/rele1/$name", "rele1", true);
  mqttClient.publish("homie/arduino/relays/rele1/$datatype", "boolean", true);
  mqttClient.publish("homie/arduino/relays/rele1/$settable", "true", true);  
    //Relay2
  mqttClient.publish("homie/arduino/relays/rele2/$name", "rele 2", true);
  mqttClient.publish("homie/arduino/relays/rele2/$datatype", "boolean", true);
  mqttClient.publish("homie/arduino/relays/rele2/$settable", "true", true);  
}

void loop(){
  
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
  digitalWrite(relay1, rele1State);
  digitalWrite(relay2, rele2State);
}

void mqttPublish(){
  float temperature = ENV.readTemperature();
  float humidity    = ENV.readHumidity();
  float pressure    = ENV.readPressure();
  float illuminance = ENV.readIlluminance();
  float uva         = ENV.readUVA();
  float uvb         = ENV.readUVB();
  float uvIndex     = ENV.readUVIndex();
  mqttClient.publish("homie/arduino/mkrenv/temperature", String(temperature).c_str(), true);  
  mqttClient.publish("homie/arduino/mkrenv/humidity", String(humidity).c_str(), true);
  mqttClient.publish("homie/arduino/mkrenv/pressure", String(pressure).c_str(), true);
  mqttClient.publish("homie/arduino/mkrenv/illuminance", String(illuminance).c_str(), true);
  mqttClient.publish("homie/arduino/mkrenv/uva", String(uva).c_str(), true);
  mqttClient.publish("homie/arduino/mkrenv/uvb", String(uvb).c_str(), true);
  mqttClient.publish("homie/arduino/mkrenv/uvindex", String(uvIndex).c_str(), true);
  mqttClient.publish("homie/arduino/mkrenv/uvb", String(uvb).c_str(), true);
  mqttClient.publish("homie/arduino/relays/rele1", String(rele1State).c_str(), true);
  mqttClient.publish("homie/arduino/relays/rele2", String(rele2State).c_str(), true);
}
