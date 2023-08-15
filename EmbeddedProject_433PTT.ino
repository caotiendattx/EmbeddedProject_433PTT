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

//void handleGetView(AsyncWebServerRequest *request) {
//  jsonBuffer.clear();
//  JsonObject& object = jsonBuffer.createObject();
//  // {
//  //   "hello": "world";
//  // }
//  object["hello"] = "world";
//  String response;
//  object.printTo(response);
//  request->send(200, "application/json", response);
//}

// example data to post:
// {
//   "key1": "value1",
//   "key2": "value2"
// }
//void handlePostBodySend(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
//      DynamicJsonBuffer jsonDynamicBuffer;
//      JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
//      if (root.success()) {
//        if (root.containsKey("key1")) {
//          Serial.println(root["key"].asString());
//        }
//        if (root.containsKey("key2")) {
//          Serial.println(root["key2"].asString());
//        }
//        request->send(200, "text/plain", "");
//      } else {
//        request->send(404, "text/plain", "");
//      }
//  }

//  //SCAN///////////////////  //SCAN///////////////////  //SCAN///////////////////  //SCAN///////////////////  //SCAN///////////////////  //SCAN/////////////////
void handlePostScanSubmit(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DynamicJsonBuffer jsonDynamicBuffer;
  JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
  
  if (root.success()) {
      tool.changeDriver(ELECHOUSE_CC1101_DRIVER);
      tool.ELECHOUSE_CC1101_SetRX();
    if (root.containsKey("modulation")) {
      tool._RFSpecs.modulation = static_cast<_433PTT_MODULATIONS>(root["modulation"].as<int>()); 
      Serial.println(String(static_cast<_433PTT_MODULATIONS>(root["modulation"].as<int>())));
    }
    if (root.containsKey("startFrequency")) {
      tool._RFSpecs.scan_start_Frequency = root["startFrequency"].as<float>();
      Serial.println(String(tool._RFSpecs.scan_start_Frequency));
    }
    if (root.containsKey("step")) {
      tool._RFSpecs.scan_increasement = root["step"].as<float>();
      Serial.println(String(tool._RFSpecs.scan_increasement));
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
//  //SCAN///////////////////  //SCAN///////////////////  //SCAN///////////////////  //SCAN///////////////////  //SCAN///////////////////  //SCAN/////////////////
//TX RX Config
void handlePostConfig(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DynamicJsonBuffer jsonDynamicBuffer;
  JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
  
  if (root.success()) {
    if (root.containsKey("modulation")) {
      tool._RFSpecs.modulation = static_cast<_433PTT_MODULATIONS>(root["modulation"].as<int>()); 
      Serial.println(String(tool._RFSpecs.modulation));
    }
    if (root.containsKey("basicFrequency")) {
      tool.operatingRFFreq = root["basicFrequency"].as<float>();
      Serial.println(String(tool.operatingRFFreq));
    }
    if (root.containsKey("frequencyDeviation")) {
      tool._RFSpecs.deviation = root["frequencyDeviation"].as<float>();
      Serial.println(String(tool._RFSpecs.deviation));
    }
    if (root.containsKey("channelNumber")) {
      tool._RFSpecs.channel = root["channelNumber"].as<int>();
      Serial.println(String(tool._RFSpecs.channel));
    }
    if (root.containsKey("channelSpacing")) {
      tool._RFSpecs.Chsp = root["channelSpacing"].as<float>();
      Serial.println(String(tool._RFSpecs.Chsp));
    }
    if (root.containsKey("receiveBW")) {
      tool._RFSpecs.RxBW = root["receiveBW"].as<float>();
      Serial.println(String(tool._RFSpecs.RxBW));
    }
    if (root.containsKey("dataRate")) {
      tool._RFSpecs.DRate = root["dataRate"].as<float>();
      Serial.println(String(tool._RFSpecs.DRate));
    }
    if (root.containsKey("power")) {
      tool._RFSpecs.TXPower = static_cast<TX_POWER>(root["power"].as<int>());
      Serial.println(String(tool._RFSpecs.TXPower));
    }
    if (root.containsKey("syncMode")) {
      tool._RFSpecs.syncMode = root["syncMode"].as<int>();
      Serial.println(String(tool._RFSpecs.syncMode));
    }
    if (root.containsKey("syncWordHigh")) {
      tool._RFSpecs.syncWord_1 = root["syncWordHigh"].as<uint32_t>();
      Serial.println(String(tool._RFSpecs.syncWord_1));
    }
    if (root.containsKey("syncWordLow")) {
      tool._RFSpecs.syncWord_2 = root["syncWordLow"].as<uint32_t>();
      Serial.println(String(tool._RFSpecs.syncWord_2));
    }
    if (root.containsKey("adrCheck")) {
      tool._RFSpecs.adrChk = root["adrCheck"].as<uint8_t>();
      Serial.println(String(tool._RFSpecs.adrChk));
    }
    if (root.containsKey("address")) {
      tool._RFSpecs.addr = root["address"].as<uint8_t>();
      Serial.println(String(tool._RFSpecs.addr));
    }
    if (root.containsKey("whitening")) {
      tool._RFSpecs.whitening = root["whitening"].as<bool>();
      Serial.println(String(tool._RFSpecs.whitening));
    }
    if (root.containsKey("pktFormat")) {
      tool._RFSpecs.pktFormat = root["pktFormat"].as<uint8_t>();
      Serial.println(String(tool._RFSpecs.pktFormat));
    }
    if (root.containsKey("lengthConfig")) {
      tool._RFSpecs.lengthConfig = root["lengthConfig"].as<uint8_t>();
      Serial.println(String(tool._RFSpecs.lengthConfig));
    }
    if (root.containsKey("packetLength")) {
      tool._RFSpecs.pktLength = root["packetLength"].as<uint8_t>();
      Serial.println(String(tool._RFSpecs.pktLength));
    }
    if (root.containsKey("crc")) {
      tool._RFSpecs.crc = root["crc"].as<bool>();
      Serial.println(String(tool._RFSpecs.crc));
    }
    if (root.containsKey("crcaf")) {
      tool._RFSpecs.crcAF = root["crcaf"].as<uint8_t>();
      Serial.println(String(tool._RFSpecs.crcAF));
    }
    if (root.containsKey("dcfilteroff")) {
      tool._RFSpecs.DCFilter = root["dcfilteroff"].as<bool>();
      Serial.println(String(tool._RFSpecs.DCFilter));
    }
    if (root.containsKey("manchester")) {
      tool._RFSpecs.manchester = root["manchester"].as<bool>();
      Serial.println(String(tool._RFSpecs.manchester));
    }
    if (root.containsKey("fec")) {
      tool._RFSpecs.FEC = root["fec"].as<int>();
      Serial.println(String(tool._RFSpecs.FEC));
    }
    if (root.containsKey("pre")) {
      tool._RFSpecs.PRE = root["pre"].as<uint8_t>();
      Serial.println(String(tool._RFSpecs.PRE));
    }
    if (root.containsKey("pqt")) {
      tool._RFSpecs.PQT = root["pqt"].as<uint8_t>();
      Serial.println(String(tool._RFSpecs.PQT));
    }
    if (root.containsKey("appendstatus")) {
      tool._RFSpecs.appendStatus = root["appendstatus"].as<bool>();
      Serial.println(String(tool._RFSpecs.appendStatus));
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

//  server.on("/on", HTTP_GET, handleGetView);

  server.on("/scan", HTTP_GET, handleGetScanRSSI);

  
//  server.on("/postDataRF", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handlePostBodySend);


  server.on("/post/scan/submit", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handlePostScanSubmit);

  
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
