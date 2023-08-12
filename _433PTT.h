#pragma once
#include "Arduino.h"
#include "cc1101_drv.h"
#include "RCSwitch_.h"

#include <stdint.h>



#define sck 18        //  GPIO 18
#define miso 19       //  GPIO 19
#define mosi 23       //  GPIO 23
#define ss 5          //  GPIO 5
#define gdo0 2        //  GPIO 2
#define gdo2 4        //  GPIO 4
#define RCSwitchPin 4

#define CCBUFFERSIZE 64
enum _433PTT_STATE{
  IDLE_STATE,
  JAMMING_STATE,
  TX_STATE,
  RX_STATE,
  SCAN_STATE,
  CONFIG_STATE
};
enum _433PTT_DRIVER{
  ELECHOUSE_CC1101_DRIVER,
  RCSWITCH_DRIVER

};

class _433PTT
{
public:
  _433PTT();
  void init();
  void signalJamming();
  void receiveRF_RCSwitch();
  bool cc1101Config(float , uint8_t , float , float , float , float );
  bool getConnectionStatus();
  void getRSSIcc1101(float, float freqStep = 0.01);
  int getState();
  void changeState(_433PTT_STATE );
  int getDriver();
  void changeDriver(_433PTT_DRIVER );
  ///////////////////////////////////////////////////
  void signalJamming(float);
  void signalJamming(float, float);
  void signalJamming(float, float, float);
  //RCSWitch Variables:
  unsigned long RCSwitch_Received_Value;
  unsigned int RCSwitch_Received_Bitlength;
  unsigned int RCSwitch_Received_Protocol;
  int RCSwitch_RSSI;
  ///////////////////////////////////////
  //RCSwitch Funct
  void mySwitch_ResetAvailable();
  void mySwitch_SetRX();
  bool mySwitch_available();
private:
  byte ccsendingbuffer[CCBUFFERSIZE] = {0};
  float operaingRFFreq;
  int8_t RSSIScanData[128];
  ELECHOUSE_CC1101 _cc1101;
  RCSwitch mySwitch;
  _433PTT_STATE _state;
  _433PTT_DRIVER _driver;


};


//  -- END OF FILE --

