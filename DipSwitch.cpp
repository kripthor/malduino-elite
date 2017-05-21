
#include "DipSwitch.h"
  
DipSwitch::DipSwitch() {
    pinMode(dip1, INPUT_PULLUP);
    pinMode(dip2, INPUT_PULLUP);
    pinMode(dip3, INPUT_PULLUP);
    pinMode(dip4, INPUT_PULLUP);
}

//dip switch 1 is the most significant bit and dip switch 4 the least
int DipSwitch::getDips(void) {
  int result = 0;
  result += !digitalRead(dip1);
  result <<= 1;
  result += !digitalRead(dip2);
  result <<= 1;
  result += !digitalRead(dip3);
  result <<= 1;
  result += !digitalRead(dip4);
  return result;
}

String DipSwitch::getDipsString(void) {
  String result = String(getDips(),BIN);
  for (int i = result.length(); i < 4; i++) result = "0"+result;
  return result;
}

