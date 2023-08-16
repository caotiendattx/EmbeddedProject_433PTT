#include "_433PTT.h"
#include <EEPROM.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"   //library for reading file
#include <ArduinoJson.h> //library for formating as JSON - version 5.13
AsyncWebServer server(80);
StaticJsonBuffer<200> jsonBuffer;
StaticJsonBuffer<200> jsonBuffer2;

String jsonString;
_433PTT tool;
void setupAPMode() {
  const char* ssid = "433PTT"; // Set the name of the access point
  const char* password = "CSEXY433"; // Set the password for the access point
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
//  //SCAN///////////////////  //SCAN///////////////////  //SCAN///////////////////  //SCAN///////////////////  //SCAN///////////////////  //SCAN/////////////////
//TX RX Config
void handlePostConfig(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DynamicJsonBuffer jsonDynamicBuffer;
  JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
  
  if (root.success()) {
    if (root.containsKey("modulation")) {
      tool._RFSpecs.modulation = static_cast<_433PTT_MODULATIONS>(root["modulation"].as<int>()); 
      Serial.println("Modulation: " + String(tool._RFSpecs.modulation));
    }
    if (root.containsKey("basicFrequency")) {
      tool.operatingRFFreq = root["basicFrequency"].as<float>();
      Serial.println("Freq: " + String(tool.operatingRFFreq));
    }
    if (root.containsKey("frequencyDeviation")) {
      tool._RFSpecs.deviation = root["frequencyDeviation"].as<float>();
      Serial.println("Freq Deviation: " + String(tool._RFSpecs.deviation));
    }
    if (root.containsKey("channelNumber")) {
      tool._RFSpecs.channel = root["channelNumber"].as<int>();
      Serial.println("Channel: " + String(tool._RFSpecs.channel));
    }
    if (root.containsKey("channelSpacing")) {
      tool._RFSpecs.Chsp = root["channelSpacing"].as<float>();
      Serial.println("CHSP: " + String(tool._RFSpecs.Chsp));
    }
    if (root.containsKey("receiveBW")) {
      tool._RFSpecs.RxBW = root["receiveBW"].as<float>();
      Serial.println("RxBW: " + String(tool._RFSpecs.RxBW));
    }
    if (root.containsKey("dataRate")) {
      tool._RFSpecs.DRate = root["dataRate"].as<float>();
      Serial.println("DRate: " + String(tool._RFSpecs.DRate));
    }
    if (root.containsKey("power")) {
      tool._RFSpecs.TXPower = static_cast<TX_POWER>(root["power"].as<int>());
      Serial.println("TXPower: " + String(tool._RFSpecs.TXPower));
    }
    if (root.containsKey("syncMode")) {
      tool._RFSpecs.syncMode = root["syncMode"].as<int>();
      Serial.println("SyncMode: " + String(tool._RFSpecs.syncMode));
    }
    if (root.containsKey("syncWordHigh")) {
      tool._RFSpecs.syncWord_1 = root["syncWordHigh"].as<uint32_t>();
      Serial.println("SyncWord High: " + String(tool._RFSpecs.syncWord_1));
    }
    if (root.containsKey("syncWordLow")) {
      tool._RFSpecs.syncWord_2 = root["syncWordLow"].as<uint32_t>();
      Serial.println("SyncWord Low: " + String(tool._RFSpecs.syncWord_2));
    }
    if (root.containsKey("adrCheck")) {
      tool._RFSpecs.adrChk = root["adrCheck"].as<uint8_t>();
      Serial.println("Address Check: " + String(tool._RFSpecs.adrChk));
    }
    if (root.containsKey("address")) {
      tool._RFSpecs.addr = root["address"].as<uint8_t>();
      Serial.println("Address: " + String(tool._RFSpecs.addr));
    }
    if (root.containsKey("whitening")) {
      tool._RFSpecs.whitening = root["whitening"].as<bool>();
      Serial.println("Data Whitening: " + String(tool._RFSpecs.whitening));
    }
    if (root.containsKey("pktFormat")) {
      tool._RFSpecs.pktFormat = root["pktFormat"].as<uint8_t>();
      Serial.println("Pkt Format: " + String(tool._RFSpecs.pktFormat));
    }
    if (root.containsKey("lengthConfig")) {
      tool._RFSpecs.lengthConfig = root["lengthConfig"].as<uint8_t>();
      Serial.println("Length Config: " + String(tool._RFSpecs.lengthConfig));
    }
    if (root.containsKey("packetLength")) {
      tool._RFSpecs.pktLength = root["packetLength"].as<uint8_t>();
      Serial.println("Pkt Length: " + String(tool._RFSpecs.pktLength));
    }
    if (root.containsKey("crc")) {
      tool._RFSpecs.crc = root["crc"].as<bool>();
      Serial.println("CRC: " + String(tool._RFSpecs.crc));
    }
    if (root.containsKey("crcaf")) {
      tool._RFSpecs.crcAF = root["crcaf"].as<uint8_t>();
      Serial.println("CRC AF: " + String(tool._RFSpecs.crcAF));
    }
    if (root.containsKey("dcfilteroff")) {
      tool._RFSpecs.DCFilter = root["dcfilteroff"].as<bool>();
      Serial.println("DC Filter: " + String(tool._RFSpecs.DCFilter));
    }
    if (root.containsKey("manchester")) {
      tool._RFSpecs.manchester = root["manchester"].as<bool>();
      Serial.println("Manchester: " + String(tool._RFSpecs.manchester));
    }
    if (root.containsKey("fec")) {
      tool._RFSpecs.FEC = root["fec"].as<int>();
      Serial.println("FEC: " + String(tool._RFSpecs.FEC));
    }
    if (root.containsKey("pre")) {
      tool._RFSpecs.PRE = root["pre"].as<uint8_t>();
      Serial.println("PRE: " + String(tool._RFSpecs.PRE));
    }
    if (root.containsKey("pqt")) {
      tool._RFSpecs.PQT = root["pqt"].as<uint8_t>();
      Serial.println("PQT: " + String(tool._RFSpecs.PQT));
    }
    if (root.containsKey("appendstatus")) {
      tool._RFSpecs.appendStatus = root["appendstatus"].as<bool>();
      Serial.println("Append Status: " + String(tool._RFSpecs.appendStatus));
    }
    if(tool.getDriver()==RCSWITCH_DRIVER){
      if(tool.getState()!=SCAN_STATE && tool.getState()!=RX_STATE ){
        tool.RCSwitch_SetRX();
      }
    }
    else{
      if(tool.getState()!=SCAN_STATE && tool.getState()!=RX_STATE ){
        tool.ELECHOUSE_CC1101_SetRX();
      }
    }    
    
    request->send(200, "text/plain", "");
    tool.cc1101UpdateConfig();
  } else {
    request->send(404, "text/plain", "");
  }
}
//TX
void handleTXSendRF(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DynamicJsonBuffer jsonDynamicBuffer;
  JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
  if (root.success()) {
      tool.changeDriver(ELECHOUSE_CC1101_DRIVER);
      tool.cc1101UpdateConfig();
    if (root.containsKey("freq1")) {
      tool._RFSpecs.modulation = static_cast<_433PTT_MODULATIONS>(root["modulation"].as<int>()); 
      Serial.println("Modulation: " + String(tool._RFSpecs.modulation));
    }
    request->send(200, "text/plain", "Send RF: OK");
  } else {
    request->send(404, "text/plain", "Send RF: ERROR");
  }
}
void handleJamConfig(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DynamicJsonBuffer jsonDynamicBuffer;
  JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
  if (root.success()) {
      tool.changeDriver(ELECHOUSE_CC1101_DRIVER);
      tool.cc1101UpdateConfig();
    if (root.containsKey("RFData")) {
      char* ptrBuffer_1 = new char[200];
      memcpy(ptrBuffer_1, root["RFData"].asString(), strlen(root["RFData"].asString()));
      tool.ELECHOUSE_CC1101_DRIVER_TX(ptrBuffer_1);
      delete [] ptrBuffer_1;
      ptrBuffer_1 = NULL;
    }
    request->send(200, "text/plain", "Send RF: OK");
  } else {
    request->send(404, "text/plain", "Send RF: ERROR");
  }
}
void handleJamState(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  DynamicJsonBuffer jsonDynamicBuffer;
  JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
  if (root.success()) {
      tool.changeDriver(ELECHOUSE_CC1101_DRIVER);
      tool.cc1101UpdateConfig();
    if (root.containsKey("state")) {
      if(root["RFData"].as<bool>() == 1){
          tool.changeState(JAMMING_STATE);
      }
      else{
          tool.changeState(IDLE_STATE);
      }
    }
    request->send(200, "text/plain", "Send RF: OK");
  } else {
    request->send(404, "text/plain", "Send RF: ERROR");
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
  server.on("/get/scan", HTTP_GET, [](AsyncWebServerRequest *request){
      String onParam = request->arg("on");
//      on = onParam.toInt() == 1;
      tool.changeState(SCAN_STATE);
      jsonBuffer.clear();
      jsonBuffer2.clear();
      JsonObject& object = jsonBuffer.createObject();
      JsonArray& rssiData = jsonBuffer2.createArray();
      for(uint8_t i = 0; i < (sizeof(tool.RSSIScanData)/sizeof(tool.RSSIScanData[0])); i++){
        rssiData.add(tool.RSSIScanData[i]);
      }
      object["freqArray"] = rssiData;
      String response;
      object.printTo(response);
      request->send(200, "application/json", response);
    });
///////////////////////////////////ReceiveRF//////////////////////////////////////////////////////////
    server.on("/get/receiverf", HTTP_GET, [](AsyncWebServerRequest *request){
      String onParam = request->arg("on");
//      on = onParam.toInt() == 1;
      tool.changeState(RX_STATE);
      tool.changeDriver(ELECHOUSE_CC1101_DRIVER);
      
//      jsonBuffer.clear();
//      JsonArray& rssiData = jsonBuffer.createArray();
//      for(uint8_t i = 0; i < (sizeof(tool.RSSIScanData)/sizeof(tool.RSSIScanData[0])); i++){
//        rssiData.add(tool.RSSIScanData[i]);
//      }
//      String response;
//      rssiData.printTo(response);
//      request->send(200, "application/json", response);
    });
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  server.on("/post/scan/submit", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handlePostScanSubmit);
  server.on("/post/txrx/submit", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handlePostConfig);
  server.on("/post/txrx/sendrf", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handleTXSendRF);
  server.on("/post/jam/config", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handleJamConfig);
  server.on("/post/jam/state", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handleJamState);

  server.begin();
}
void setup() {
  Serial.begin(115200); //define frequency of serial monitor
  setupAPMode();
  setUpRoutes();
  if (tool.getConnectionStatus()) {  // Check the CC1101 Spi connection.
      Serial.println(F("CC1101 Connection OK\n\r"));
  } else {
      Serial.println(F("CC1101 Connection Error\n\r"));
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
