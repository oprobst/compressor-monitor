
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

#define Q1_C 8192
#define Q2_C 8193
#define Q3_C 8194
#define Q4_C 8195
#define Q5_C 8196
#define Q6_C 8197
#define Q7_C 8198
#define Q8_C 8199
#define Q9_C 8200
#define Q10_C 8201
#define Q11_C 8202
#define Q12_C 8203
#define Q13_C 8204
#define Q14_C 8205
#define Q15_C 8206
#define Q16_C 8207

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


#define M1_C 8256
#define M2_C 8257
#define M3_C 8258
#define M4_C 8259
#define M5_C 8260
#define M6_C 8261
#define M7_C 8262
#define M8_C 8263
#define M9_C 8264
#define M10_C 8265
#define M11_C 8266
#define M12_C 8267
#define M13_C 8268
#define M14_C 8269
#define M15_C 8270
#define M16_C 8271
#define M17_C 8272
#define M18_C 8273
#define M19_C 8274
#define M20_C 8275
#define M21_C 8276
#define M22_C 8277
#define M23_C 8278
#define M24_C 8279
#define M25_C 8280
#define M26_C 8281
#define M27_C 8282
#define M28_C 8283
#define M29_C 8284
#define M30_C 8285

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include <Ethernet.h>
#include "Data.h"
#include "DebugSerial.h"

extern Data data;

class Logo
{
private:
    ModbusTCPClient *modbusTCPClient;

    long cache[32];

    bool checkCache(long *resultbuffer, int index, int address);
    bool checkCache(bool *resultbuffer, int index, int address);

public:
    bool connect(ModbusTCPClient *modbusTCPClient);

    bool readPressureAir(long *value);
    bool readPressureMix(long *value);

    bool readRoomTemp(long *value);
    bool readCompressorStage1Temp(long *value);
    bool readCompressorStage2Temp(long *value);
    bool readCompressorStage3Temp(long *value);

    bool readMaintenanceSwitch(bool *value);
    bool readSummerSwitch(bool *value);
    bool readRollershutter(bool *value);
    bool readVentilationInside(bool *value);
    bool readVentilationOutside(bool *value);
    bool readMixerValve(bool *value);
    bool readHeater(bool *value);
    bool readCompressorRunning(bool *value);

    bool readAlertKompressorstoerung(bool *value);
    bool readAlertFilterwechsel(bool *value);
    bool readAlertTempRoom(bool *value);
    bool readAlertTempStage1(bool *value);
    bool readAlertTempStage2(bool *value);
    bool readAlertTempStage3(bool *value);
    bool readAlertSpsOffline(bool *value);
    bool readAlertEmergencyStop(bool *value);

    void resetCache();
};

#endif