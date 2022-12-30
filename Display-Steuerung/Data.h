
#ifndef DATA_H_
#define DATA_H_

#include <Arduino.h>

struct Data
{
    bool dhcpSuccess;
    bool connected;
    bool modbusConnected;
    String wifiShieldType;
};

#endif