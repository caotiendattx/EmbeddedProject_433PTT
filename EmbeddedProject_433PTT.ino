#include "_433PTT.h"
#include <EEPROM.h>
#include "WiFi.h"
//#include "ESPAsyncWebServer.h"
//#include "SPIFFS.h"   //library for reading file
#include <ArduinoJson.h> //library for formating as JSON
#define EPROMSIZE 512              // Size of EEPROM in your Arduino chip. For ESP32 it is Flash simulated so very slow

_433PTT tool;

void setup() {
  Serial.begin(115200);
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
void handle_Scan_State(){
  float start_Frequency;
  float increasement;
  tool.getRSSIcc1101(start_Frequency, increasement);
  //TODO: DATA TO FE
  // tool.RSSIScanData[i]; 
}



void loop() {
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
      handle_Scan_State();
    break;
    case JAMMING_STATE:
      tool.signalJamming();
    break;
    default:
    break;
  }
}