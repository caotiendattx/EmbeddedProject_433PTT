#include "_433PTT.h"
  void _433PTT::cc1101Config(){
        // initializing library with custom pins selected
     _cc1101.setSpiPin(sck, miso, mosi, ss);
     _cc1101.setGDO(gdo0, gdo2);

    // Main part to tune CC1101 with proper frequency, modulation and encoding    
    _cc1101.Init();                // must be set to initialize the cc1101!
    _cc1101.setGDO0(gdo0);         // set lib internal gdo pin (gdo0). Gdo2 not use for this example.
    _cc1101.setCCMode(1);          // set config for internal transmission mode. value 0 is for RAW recording/replaying
    _cc1101.setModulation(2);      // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    _cc1101.setMHZ(433.92);        // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    _cc1101.setDeviation(47.60);   // Set the Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz.
    _cc1101.setChannel(0);         // Set the Channelnumber from 0 to 255. Default is cahnnel 0.
    _cc1101.setChsp(199.95);       // The channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz.
    _cc1101.setRxBW(812.50);       // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
    _cc1101.setDRate(9.6);         // Set the Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud!
    _cc1101.setPA(10);             // Set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
    _cc1101.setSyncMode(2);        // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
    _cc1101.setSyncWord(211, 145); // Set sync word. Must be the same for the transmitter and receiver. Default is 211,145 (Syncword high, Syncword low)
    _cc1101.setAdrChk(0);          // Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast.
    _cc1101.setAddr(0);            // Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF).
    _cc1101.setWhiteData(0);       // Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on.
    _cc1101.setPktFormat(0);       // Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.
    _cc1101.setLengthConfig(1);    // 0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved
    _cc1101.setPacketLength(0);    // Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed.
    _cc1101.setCrc(0);             // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
    _cc1101.setCRC_AF(0);          // Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size.
    _cc1101.setDcFilterOff(0);     // Disable digital DC blocking filter before demodulator. Only for data rates ≤ 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity).
    _cc1101.setManchester(0);      // Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.
    _cc1101.setFEC(0);             // Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable.
    _cc1101.setPRE(0);             // Sets the minimum number of preamble bytes to be transmitted. Values: 0 : 2, 1 : 3, 2 : 4, 3 : 6, 4 : 8, 5 : 12, 6 : 16, 7 : 24
    _cc1101.setPQT(0);             // Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4∙PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted.
    _cc1101.setAppendStatus(0);    // When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK.
  }
///////////////////////////////////////////////
// convert bytes in table to string with hex numbers
void asciitohex(byte *ascii_ptr, byte *hex_ptr,int len)
{
    byte i,j,k;
    for(i = 0; i < len; i++)
    {
      // high byte first
      j = ascii_ptr[i] / 16;
      if (j>9) 
         { k = j - 10 + 65; }
      else 
         { k = j + 48; }
      hex_ptr[2*i] = k ;
      // low byte second
      j = ascii_ptr[i] % 16;
      if (j>9) 
         { k = j - 10 + 65; }
      else
         { k = j + 48; }
      hex_ptr[(2*i)+1] = k ; 
    };
    hex_ptr[(2*i)+2] = '\0' ; 
}
void  hextoascii(byte *ascii_ptr, byte *hex_ptr,int len)
{
    byte i,j;
    for(i = 0; i < (len/2); i++)
     { 
     j = hex_ptr[i*2];
     if ((j>47) && (j<58))  ascii_ptr[i] = (j - 48) * 16;
     if ((j>64) && (j<71))  ascii_ptr[i] = (j - 55) * 16;
     if ((j>96) && (j<103)) ascii_ptr[i] = (j - 87) * 16;
     j = hex_ptr[i*2+1];
     if ((j>47) && (j<58))  ascii_ptr[i] = ascii_ptr[i]  + (j - 48);
     if ((j>64) && (j<71))  ascii_ptr[i] = ascii_ptr[i]  + (j - 55);
     if ((j>96) && (j<103)) ascii_ptr[i] = ascii_ptr[i]  + (j - 87);
     };
    ascii_ptr[i++] = '\0' ;
}
///////////////////////////////////////////////
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
void _433PTT::RCSwitch_SetRX(){
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
  for(uint8_t i=0; i < 128; i++){
    _cc1101.setMHZ(freqSet);
    RSSIScanData[i] = _cc1101.getRssi();
    Serial.print(RSSIScanData[i]);
    Serial.print(" ");
    freqSet += freqStep;
  }
}

bool _433PTT::RCSwitch_available(){
  return mySwitch.available();
}

void _433PTT::receiveRF_RCSwitch(){
  RCSwitch_Received_Value = mySwitch.getReceivedValue();
  RCSwitch_Received_Bitlength = mySwitch.getReceivedBitlength();
  RCSwitch_Received_Protocol = mySwitch.getReceivedProtocol();
  RCSwitch_RSSI = _cc1101.getRssi();
  mySwitch.resetAvailable();
}

bool _433PTT::ELECHOUSE_CC1101_DRIVER_RX(){
  if (_cc1101.CheckCRC())
  {
    //Get received Data and calculate length
    int len = _cc1101.ReceiveData(ccreceivingbuffer);
    ccreceivingbuffer[len] = '\0';
    // flush textbuffer
    for (int i = 0; i < BUF_LENGTH; i++)
    { textbuffer[i] = 0; };
    asciitohex(ccreceivingbuffer, textbuffer,  len);
    // Serial.print((char *)textbuffer);
    _cc1101.SetRx();
    return 1;
  }
  else return 0;
}
void _433PTT::ELECHOUSE_CC1101_DRIVER_TX(char* send_buffer){
  hextoascii(ccsendingbuffer,(byte *)send_buffer, strlen(send_buffer));  
  ccsendingbuffer[strlen(send_buffer)/2] = 0x00;
  _cc1101.SendData(ccsendingbuffer, (byte)(strlen(send_buffer)/2));
}
bool _433PTT::ELECHOUSE_CC1101_DRIVER_CheckReceiveFlag(){
  return _cc1101.CheckReceiveFlag();
}
void _433PTT::ELECHOUSE_CC1101_SetRX(){
  _cc1101.SetRx();
}





