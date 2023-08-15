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
#define BUF_LENGTH 128             // Buffer for the incoming command.

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
enum _433PTT_MODULATIONS{
  _2_FSK = 0,
  GFSK,
  ASK_OOK,
  _4_FSK,
  MSK
};
enum TX_POWER {
    MINUS_30 = -30,
    MINUS_20 = -20,
    MINUS_15 = -15,
    MINUS_10 = -10,
    MINUS_6 = -6,
    ZERO = 0,
    PLUS_5 = 5,
    PLUS_7 = 7,
    PLUS_10 = 10,
    PLUS_11 = 11,
    PLUS_12 = 12
};
struct RF_Specs{
  bool CCMode;
  _433PTT_MODULATIONS modulation;
  float deviation;
  uint8_t channel;
  float Chsp;
  float RxBW;
  float DRate;
  TX_POWER TXPower;
  uint8_t syncMode;
  uint32_t syncWord_1;
  uint32_t syncWord_2;
  uint8_t adrChk;
  uint8_t addr;
  bool whitening;
  uint8_t pktFormat;
  uint8_t lengthConfig;
  uint8_t pktLength;
  uint8_t crcAF;
  bool crc;
  bool DCFilter;
  bool manchester;
  bool FEC;
  uint8_t PRE;
  uint8_t PQT;
  bool appendStatus;
  float scan_start_Frequency;
  float scan_increasement;
};
class _433PTT
{
public:
  _433PTT();
  void init();
  void signalJamming();
  void receiveRF_RCSwitch();
  bool cc1101Config(float , uint8_t , float , float , float , float );
  void cc1101Config();
  void cc1101UpdateConfig();
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
  void RCSwitch_SetRX();
  bool RCSwitch_available();
  //ELECHOUSE Funct
  bool ELECHOUSE_CC1101_DRIVER_RX();
  void ELECHOUSE_CC1101_SetRX();
  void ELECHOUSE_CC1101_DRIVER_TX(char*);
  bool ELECHOUSE_CC1101_DRIVER_CheckReceiveFlag();
  //ELECHOUSE Variables:
  float operatingRFFreq;
  RF_Specs _RFSpecs;
  //Public Variables:
  int8_t RSSIScanData[128];
  byte ccsendingbuffer[CCBUFFERSIZE] = {0};
  byte ccreceivingbuffer[CCBUFFERSIZE] = {0};
  // buffer for hex to ascii conversions 
  byte textbuffer[BUF_LENGTH];
  //SCAN

private:
  ELECHOUSE_CC1101 _cc1101;
  RCSwitch mySwitch;
  _433PTT_STATE _state;
  _433PTT_DRIVER _driver;
};


//  -- END OF FILE --
