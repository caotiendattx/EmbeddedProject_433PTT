#include "_433PTT.h"
#include <EEPROM.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"   //library for reading file
#include <ArduinoJson.h> //library for formating as JSON - version 5.13

#define EPROMSIZE 512              // Size of EEPROM in your Arduino chip. For ESP32 it is Flash simulated so very slow

AsyncWebServer server(80);
StaticJsonBuffer<200> jsonBuffer;
String jsonString;
_433PTT tool;
void setupAPMode() {
  const char* ssid = "433PTT"; // Set the name of the access point
  const char* password = "12345678"; // Set the password for the access point
  WiFi.softAP(ssid, password); // Set the ESP32 to AP mode with the specified SSID and password
  Serial.print("Access point IP address: ");
  Serial.println(WiFi.softAPIP()); // Print the IP address of the access point
}

void handleGetView(AsyncWebServerRequest *request) {
  jsonBuffer.clear();
  JsonObject& object = jsonBuffer.createObject();
  // {
  //   "hello": "world";
  // }
  object["hello"] = "world";
  String response;
  object.printTo(response);
  request->send(200, "application/json", response);
}

// example data to post:
// {
//   "key1": "value1",
//   "key2": "value2"
// }
void handlePostBodySend(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      DynamicJsonBuffer jsonDynamicBuffer;
      JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
      if (root.success()) {
        if (root.containsKey("key1")) {
          Serial.println(root["key"].asString());
        }
        if (root.containsKey("key2")) {
          Serial.println(root["key2"].asString());
        }
        request->send(200, "text/plain", "");
      } else {
        request->send(404, "text/plain", "");
      }
  }

//  //SCAN
void handlePostScan(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DynamicJsonBuffer jsonDynamicBuffer;
  JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
  
  if (root.success()) {
      tool.changeState(SCAN_STATE);
      tool.changeDriver(ELECHOUSE_CC1101_DRIVER);
    if (root.containsKey("scan_modulation")) {
      tool._RFSpecs.modulation = static_cast<_433PTT_MODULATIONS>(root["scan_modulation"].as<int>()); 
      Serial.println("Modulation: " + String(root["scan_modulation"].as<int>()));
    }
    if (root.containsKey("scan_start_frequency")) {
      tool._RFSpecs.scan_start_Frequency = static_cast<_433PTT_MODULATIONS>(root["scan_start_frequency"].as<int>());
      Serial.println("Start Frequency: " + String(root["scan_start_frequency"].as<int>()));
    }
    if (root.containsKey("scan_step")) {
      tool._RFSpecs.scan_increasement = static_cast<_433PTT_MODULATIONS>(root["scan_step"].as<int>());
      Serial.println("Start Frequency: " + String(root["scan_step"].as<int>()));
    }
    request->send(200, "text/plain", "");
    tool.cc1101UpdateConfig();
  } else {
    request->send(404, "text/plain", "");
  }
}
void handleGetScanRSSI(AsyncWebServerRequest *request) {
  tool.changeState(SCAN_STATE);
  jsonBuffer.clear();
  // create an empty array
  JsonArray& rssiData = jsonBuffer.createArray();
    for(uint8_t i = 0; i < (sizeof(tool.RSSIScanData)/sizeof(tool.RSSIScanData[0])); i++){
        rssiData.add(tool.RSSIScanData[i]);
      }
  String response;
  rssiData.printTo(response);
  request->send(200, "application/json", response);
}
void handlePostScanOFF(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DynamicJsonBuffer jsonDynamicBuffer;
  JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
  
  if (root.success()) {
      tool.changeState(IDLE_STATE);
    if (root.containsKey("scan_off")) {
      
      Serial.println("Scan: OFF");
    }
    request->send(200, "text/plain", "");
    tool.cc1101UpdateConfig();
  } else {
    request->send(404, "text/plain", "");
  }
}

//TX RX Config
void handlePostConfig(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DynamicJsonBuffer jsonDynamicBuffer;
  JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
  
  if (root.success()) {
    if (root.containsKey("scan_modulation")) {
      tool._RFSpecs.modulation = static_cast<_433PTT_MODULATIONS>(root["scan_modulation"].as<int>()); 
      Serial.println("Modulation: " + String(root["scan_modulation"].as<int>()));
    }
    if (root.containsKey("scan_start_frequency")) {
      tool._RFSpecs.scan_start_Frequency = static_cast<_433PTT_MODULATIONS>(root["scan_start_frequency"].as<int>());
      Serial.println("Start Frequency: " + String(root["scan_start_frequency"].as<int>()));
    }
    if (root.containsKey("scan_step")) {
      tool._RFSpecs.scan_increasement = static_cast<_433PTT_MODULATIONS>(root["scan_step"].as<int>());
      Serial.println("Start Frequency: " + String(root["scan_step"].as<int>()));
    }
    request->send(200, "text/plain", "");
    tool.cc1101UpdateConfig();
  } else {
    request->send(404, "text/plain", "");
  }
}
void setUpRoutes(){
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.html", String(), false);
  });
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/styles.css", "text/css");
  });
  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.js", "application/javascript");
  });

  server.on("/on", HTTP_GET, handleGetView);

  server.on("/scan", HTTP_GET, handleGetScanRSSI);

  
  server.on("/postDataRF", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handlePostBodySend);


  server.on("/post/scan/submit", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handlePostScan);
  server.on("/post/scan/off", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handlePostScanOFF);
  
  server.begin();
}
void setup() {
  Serial.begin(115200); //define frequency of serial monitor
  Serial.println("successfully");
  setupAPMode();
  setUpRoutes();
  EEPROM.begin(EPROMSIZE);
  if (tool.getConnectionStatus()) {  // Check the CC1101 Spi connection.
      Serial.println(F("Connection OK\n\r"));
  } else {
      Serial.println(F("Connection Error\n\r"));
  };
  tool.init();
  tool.changeState(IDLE_STATE);
}

void handle_RX_State(){
  if(tool.getDriver()==RCSWITCH_DRIVER){
    if(tool.getState()!=SCAN_STATE && tool.getState()!=RX_STATE ){
      tool.RCSwitch_SetRX();
    }

    if(tool.RCSwitch_available()){
      tool.receiveRF_RCSwitch();
      //TODO: DATA TO FE
      // tool.RCSwitch_Received_Value
      // tool.RCSwitch_Received_Bitlength
      // tool.RCSwitch_Received_Protocol
      // tool.RCSwitch_RSSI
    }
  }
  else if(tool.getDriver()==ELECHOUSE_CC1101_DRIVER){
    
    if(tool.getState()!=SCAN_STATE && tool.getState()!=RX_STATE ){
      tool.ELECHOUSE_CC1101_SetRX();
    }
    
    if(tool.ELECHOUSE_CC1101_DRIVER_CheckReceiveFlag()){
      tool.ELECHOUSE_CC1101_DRIVER_RX();
      //TODO: DATA TO FE 
      // tool.ccreceivingbuffer
      // tool.textbuffer
    }
  }
}
void embedded_app(){

    switch(tool.getState()){
    case IDLE_STATE:

    break;
    case TX_STATE:
      // char* send_buffer;
      //  char* send_buffer = "A123";
      // tool.ELECHOUSE_CC1101_DRIVER_TX(send_buffer);

    break;
    case RX_STATE:
      handle_RX_State();

    break;
    case SCAN_STATE:
      tool.getRSSIcc1101(tool._RFSpecs.scan_start_Frequency, tool._RFSpecs.scan_increasement);
    break;
    case JAMMING_STATE:
      tool.signalJamming();
    break;
    default:
    break;
  }
}
void loop() {
  embedded_app();
}
