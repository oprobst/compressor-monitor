
#include "Logo.h"
#include "DebugSerial.h"

#include "Data.h"

extern byte serverIp[];
extern int serverPort;

void Logo::resetCache()
{
  roomTemperature = -274;
  comp1Temp = -274;
  comp2Temp = -274;
  comp3Temp = -274;

  emergencyOffSwitch = NULL;
  summerSwitch = false;
  humidityAlert = false;
  rollershutter = false;
  ventilationInside = false;
  ventilationOutside = false;
  heater = false;
  kompressorRunning = false;
}

bool Logo::connect(ModbusTCPClient * modbusTCPClient)
{
  this->modbusTCPClient = modbusTCPClient;

  while (!modbusTCPClient->connected())
  {
    SPRINT("Trying to connect to Modbus Server at ");
    SPRINT(IPAddress(serverIp));
    SPRINT(":");
    SPRINT(serverPort);
    SPRINT("...");
    if (!modbusTCPClient->begin(IPAddress(serverIp), serverPort))
    {
      data.connected = false;
      SPRINTLN(" [FAILED]");
      return false;
    }
    else
    {
      SPRINTLN(" [SUCCESS]");
      data.connected = true;
      return true;
    }
  }
}

bool Logo::readRoomTemp(float *value)
{
  long i = modbusTCPClient->holdingRegisterRead(AM1_HR);
  bool changed = (roomTemperature != i);
  if (changed)
    roomTemperature = i;
  *value = i / 10.0f;
  return changed;
}

bool Logo::readEmergencyOffSwitch(bool *value)
{
  bool i = modbusTCPClient->discreteInputRead(I3_DI);
  bool changed = (emergencyOffSwitch != i);
  if (changed)
    emergencyOffSwitch = i;
  *value = emergencyOffSwitch;
  return changed;
}

bool Logo::readMaintenanceSwitch(bool *value)
{
  bool i = modbusTCPClient->discreteInputRead(I4_DI);
  bool changed = (maintenanceSwitch != i);
  if (changed)
    maintenanceSwitch = i;
  *value = maintenanceSwitch;
  return changed;
}

bool Logo::readCompressorStage1Temp(float *value)
{
  long i = modbusTCPClient->holdingRegisterRead(AM2_HR);
  bool changed = (comp1Temp != i);
  if (changed)
    comp1Temp = i;
  *value = i / 10.0f;
  return changed;
}

bool Logo::readCompressorStage2Temp(float *value)
{
  long i = modbusTCPClient->holdingRegisterRead(AM3_HR);
  bool changed = (comp2Temp != i);
  if (changed)
    comp2Temp = i;
  *value = i / 10.0f;
  return changed;
}

bool Logo::readCompressorStage3Temp(float *value)
{
  long i = modbusTCPClient->holdingRegisterRead(AM4_HR);
  bool changed = (comp3Temp != i);
  if (changed)
    comp3Temp = i;
  *value = i / 10.0f;
  return changed;
  }


/*
float offdeeleteme()
{

  c1 = modbusTCPClient.inputRegisterRead(AI1_IR);
  if (c1o != c1)
  {
    SPRINT("c1: ");
    SPRINTLN(c1);
    c1o = c1;
  }
  // SPRINT (i);
  c2 = modbusTCPClient.holdingRegisterRead(AM1_HR);
  if (c2o != c2)
  {
    //   SPRINT (i);
    SPRINT("c2: ");
    SPRINTLN(c2);
    c2o = c2;
    ui.showRoomTemp(c2 / 10.0f);
  }

  c3 = modbusTCPClient.discreteInputRead(I1_DI);
  if (c3o != c3)
  {
    SPRINT("c3: ");
    SPRINTLN(c3);
    c3o = c3;
  }
  delay(333);
  // ui.renderSystemInfo();
}
*/