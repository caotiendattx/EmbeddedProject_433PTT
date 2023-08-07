#pragma once
#include "Arduino.h"
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <stdint.h>



#define sck 18        //  GPIO 18
#define miso 19       //  GPIO 19
#define mosi 23       //  GPIO 23
#define ss 5          //  GPIO 5
#define gdo0 2        //  GPIO 2
#define gdo2 4        //  GPIO 4

#define CCBUFFERSIZE 64

struct RFData{
  char* name;
  uint32_t signalInBinary;
};

class _433PTT
{
public:
  _433PTT();
  RFData getSavedRF(const char* name);
  bool saveRF(RFData _rf);
  bool deteteSavedRF(RFData _rf);
  bool sendRF(RFData);
  bool readRF();
  bool signalJamming();
  bool cc1101Init(float _frequency = 433.92, uint8_t _modulation = 2, float _deviation = 47.60, float _RXBW = 812.50, float _chsp = 199.95, float _DRate = 9.6);
  bool getConnectionStatus();
private:
  byte ccsendingbuffer[CCBUFFERSIZE] = {0};
  static operaingRFFreq;


};


//  -- END OF FILE --

