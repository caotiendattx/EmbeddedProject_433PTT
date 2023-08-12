#include "_433PTT.h"


_433PTT::_433PTT(){
  mySwitch = RCSwitch();
  _state = IDLE_STATE;
}
int _433PTT::getState(){
  return _state;
}
void _433PTT::changeState(_433PTT_STATE state){
  _state = state;
}
int _433PTT::getDriver(){
  return _driver;
}
void _433PTT::changeDriver(_433PTT_DRIVER driver){
  _driver = driver;
}
void _433PTT::mySwitch_SetRX(){
  _cc1101.SetRx();
}

void _433PTT::init(){
  _cc1101.setSpiPin(sck, miso, mosi, ss);
  _cc1101.setGDO(gdo0, gdo2);
  _cc1101.Init();
  _cc1101.setGDO0(gdo0);      
  _cc1101.setMHZ(433.92); 
  mySwitch.enableReceive(RCSwitchPin);
}

bool _433PTT::cc1101Config(float _frequency , uint8_t _modulation , float _deviation , float _RXBW , float _chsp , float _DRate ){
    _cc1101.setCCMode(1);          // set config for internal transmission mode. value 0 is for RAW recording/replaying
    _cc1101.setModulation(_modulation);      // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    _cc1101.setMHZ(_frequency);        // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    _cc1101.setDeviation(_deviation);   // Set the Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz.
    _cc1101.setChannel(0);         // Set the Channelnumber from 0 to 255. Default is cahnnel 0.
    _cc1101.setChsp(_chsp);       // The channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz.
    _cc1101.setRxBW(_RXBW);       // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
    _cc1101.setDRate(_DRate);         // Set the Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud!
    _cc1101.setPA(10);             // Set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
    return 1;
}

bool _433PTT::getConnectionStatus(){
  if (_cc1101.getCC1101()) {  // Check the CC1101 Spi connection.
      return 1;
  } else {
      return 0;
    };
}

void _433PTT::signalJamming(){
  randomSeed(analogRead(0));
  for (int i = 0; i<60; i++)
    {
        ccsendingbuffer[i] = (byte)random(255);
    };
  _cc1101.SendData(ccsendingbuffer,60);
}

void _433PTT::getRSSIcc1101(float freqSet, float freqStep){
  if(_state!=SCAN_STATE && _state!=RX_STATE){
    _cc1101.SetRx();
  }
  for(uint8_t i=0; i < 128; i++){
    _cc1101.setMHZ(freqSet);
    RSSIScanData[i] = _cc1101.getRssi();
    Serial.print(RSSIScanData[i]);
    Serial.print(" ");
    freqSet += freqStep;
  }
}
void _433PTT::mySwitch_ResetAvailable(){
  mySwitch.resetAvailable();
}
bool _433PTT::mySwitch_available(){
  return mySwitch.available();
}

void _433PTT::receiveRF_RCSwitch(){
    RCSwitch_Received_Value = mySwitch.getReceivedValue();
    RCSwitch_Received_Bitlength = mySwitch.getReceivedBitlength();
    RCSwitch_Received_Protocol = mySwitch.getReceivedProtocol();
    RCSwitch_RSSI = _cc1101.getRssi(); 
}



