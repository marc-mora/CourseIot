#include <WiFi101.h>
#include <PubSubClient.h>
#include <string.h>
#include <ArduinoJson.h>



int status = WL_IDLE_STATUS;


void callback(char* topicBack, byte* payload, unsigned int length) {
  String topicStr = topicBack;
    int i=0;
    String payloadChange = "";
    for (i=0;i<length;i++) {
      payloadChange = payloadChange + (char)payload[i];
    }
    if(String(topicStr) == "homie/arduino-1/callback/homie"){
        topic = "homie/arduino-1/homie";
        publicar("$homie", "4.0.0", topic);
    }

    if(topicStr == "homie/arduino-1/callback/$name"){
      
    }
      
    if(topicStr == "homie/arduino-1/callback/$state"){
      
    }
        
    if(topicStr == "homie/arduino-1/callback/$nodes"){
     
    }

    if(topicStr == "homie/arduino-1/callback/$implementation"){
    }
        
    
    if(topicStr == "homie/arduino-1/callback/sensores/$name"){
    }
        
    if(topicStr == "homie/arduino-1/callback/sensores/$type"){
    } 
        
    if(topicStr == "homie/arduino-1/callback/sensores/$properties"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/temperatura"){
    }
        
    if(topicStr == "homie/arduino-1/callback/sensores/temperatura/$name"){
    }
      
    if(topicStr == "homie/arduino-1/callback/sensores/temperatura/$datatype"){
    }   
    
    if(topicStr == "homie/arduino-1/callback/sensores/temperatura/$settable"){
    }
        
    if(topicStr == "homie/arduino-1/callback/sensores/temperatura/$unit"){
    }    
    
    if(topicStr == "homie/arduino-1/callback/sensores/humedad"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/humedad/$name"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/humedad/$datatype"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/humedad/$settable"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/humedad/$unit"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/presion"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/presion/$name"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/presion/$datatype"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/presion/$settable"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/presion/$unit"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/uva"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/uva/$name"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/uva/$datatype"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/uva/$settable"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/uva/$unit"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/uvb"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/uvb/$name"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/uvb/$datatype"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/uvb/$settable"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/uvb/$unit"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/indice-uv"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/indice-uv/$name"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/indice-uv/$datatype"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/indice-uv/$settable"){
    }
    
    if(topicStr == "homie/arduino-1/callback/sensores/indice-uv/$unit"){
    }
    
}

WiFiClient wificlient;
PubSubClient mqttclient(mqttServer, mqttPort, callback, wificlient);

void publicar(String subtopic, String medida, char* topicpublicar){
  StaticJsonDocument<200> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();
  root["label"] = subtopic;
  root["data"] = medida;
  String dataStr = "";
  serializeJson(root, dataStr);
  //Serial.println(dataStr);
  size_t len = dataStr.length();
  size_t size = len+1;
  
  byte message[size];
  dataStr.getBytes(message, size);

  mqttclient.publish(topicpublicar,(char*) message);  
}

void reconnect() {
  if(WiFi.status() != WL_CONNECTED){
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }

  if(WiFi.status() == WL_CONNECTED) {
    while (!mqttclient.connected()) {
      if (mqttclient.connect(deviceId, userBroker, passBroker)) {
        Serial.print("\tMTQQ Conectado");
        mqttclient.subscribe(callbackTopic);
      }
      else{
        delay(500);
        Serial.println("tFailed.");
        reconnect();
      }
    }
  }
}

void setup_MQTT() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  Serial.println("Connected to wifi");
  reconnect();
}


void loop_MQTT() {
  if (!mqttclient.connected()) {
    reconnect();
  }else{
    mqttclient.loop();
  }
  
}
