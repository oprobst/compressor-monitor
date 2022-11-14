
#ifndef LOGO_H_
#define LOGO_H_

// Register definition
#define I1_DI 0
#define I2_DI 1
#define I3_DI 2
#define I4_DI 3
#define I5_DI 4
#define I6_DI 5
#define I7_DI 6
#define I8_DI 7
#define I9_DI 8
#define I10_DI 9
#define I11_DI 10
#define I12_DI 11
#define I13_DI 12
#define I14_DI 13
#define I15_DI 14
#define I16_DI 15

#define AI1_IR 0
#define AI2_IR 1
#define AI3_IR 2
#define AI4_IR 3
#define AI5_IR 4
#define AI6_IR 5
#define AI7_IR 6
#define AI8_IR 7

#define AM1_HR 528
#define AM2_HR 529
#define AM3_HR 530
#define AM4_HR 531
#define AM5_HR 532
#define AM6_HR 533
#define AM7_HR 534
#define AM8_HR 535
#define AM9_HR 536
#define AM10_HR 537

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include <Ethernet.h>
#include "Data.h"
#include "Config.h"

extern Data data;

class Logo
{
private:
    ModbusTCPClient *modbusTCPClient;
    long roomTemperature = -274;
    long comp1Temp = -274;
    long comp2Temp = -274;
    long comp3Temp = -274;

    
    bool emergencyOffSwitch = NULL;
    bool maintenanceSwitch = false;
    bool summerSwitch = false;
    bool humidityAlert = false;
    bool rollershutter = false;
    bool ventilationInside = false;
    bool ventilationOutside = false;
    bool mixerValve = false;
    bool heater = false;
    bool kompressorRunning = false;


public:
    bool connect(ModbusTCPClient * modbusTCPClient);
    bool readRoomTemp(float *value);

    bool readCompressorStage1Temp (float * value);    
    bool readCompressorStage2Temp (float * value);    
    bool readCompressorStage3Temp (float * value);
     
    bool readEmergencyOffSwitch (bool * value);
    bool readMaintenanceSwitch (bool * value);
    bool readSummerSwitch (bool * value);
    bool readHumidityAlert (bool * value);
    bool readRollershutter (bool * value);
    bool readVentilationInside (bool * value);
    bool readVentilationOutside (bool * value);
    bool readMixerValve (bool * value);
    bool readHeater (bool * value);
    bool readKompressorRunning (bool * value);

    void resetCache();


};

#endif