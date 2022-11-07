
#ifndef DATA_H_
#define DATA_H_

#include <Arduino.h>

struct Data
{
    bool dhcpSuccess;
    bool connected;
    String wifiShieldType;

};

#endif