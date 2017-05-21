#ifndef DIPSWITCH_h
#define DIPSWITCH_h

#include "Arduino.h"

//Dip-Switch Pins:
#define dip1 6
#define dip2 8
#define dip3 9
#define dip4 10

class DipSwitch
{
  public:
    DipSwitch(void);
    int getDips(void);
    String getDipsString(void);
    
};

#endif
