#include "_433PTT.h"
#include <EEPROM.h>
#define EPROMSIZE 512              // Size of EEPROM in your Arduino chip. For ESP32 it is Flash simulated so very slow


  _433PTT tool;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EPROMSIZE);
  tool.cc1101Init();
  if (tool.getConnectionStatus()) {  // Check the CC1101 Spi connection.
      Serial.println(F("cc1101 initialized. Connection OK\n\r"));
  } else {
      Serial.println(F("cc1101 connection error! check the wiring.\n\r"));
  };
  ELECHOUSE_cc1101.SetRx();  // set Receive on

}

void loop() {

}