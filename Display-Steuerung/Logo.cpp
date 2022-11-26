#include "Logo.h"

extern byte serverIp[];
extern int serverPort;

void Logo::resetCache()
{
  for (int i = 0; i<32; i ++){
    if (i != 7 && i != 15) // dont reset mixer mode and maintenance
    cache [i] = -999;
  } 
}

bool Logo::connect(ModbusTCPClient *clientToConnect)
{
  this->modbusTCPClient = clientToConnect;

  if (!modbusTCPClient->connected())
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
  SPRINTLN("Modbus already connected.");
}

// Read analog values
bool Logo::readPressureAir(long *value)
{
  return checkCache(value, 0, AM1_HR);
}
bool Logo::readPressureMix(long *value)
{
  return checkCache(value, 1, AM2_HR);
}

bool Logo::readRoomTemp(long *value)
{
  return checkCache(value, 2, AM3_HR);
}

bool Logo::readCompressorStage1Temp(long *value)
{
  return checkCache(value, 3, AM4_HR);
}

bool Logo::readCompressorStage2Temp(long *value)
{
  return checkCache(value, 4, AM5_HR);
}

bool Logo::readCompressorStage3Temp(long *value)
{
  return checkCache(value, 5, AM6_HR);
}

// read boolean values
bool Logo::readAlertEmergencyStop(bool *value)
{
  return checkCache(value, 6, M10_C);
}

bool Logo::readMaintenanceSwitch(bool *value)
{
  return checkCache(value, 7, I4_DI);
}

bool Logo::readAlertKompressorstoerung(bool *value)
{
  return checkCache(value, 8, M11_C);
}
bool Logo::readAlertFilterwechsel(bool *value)
{
  return checkCache(value, 9, M9_C);
}
bool Logo::readAlertTempRoom(bool *value)
{
  return checkCache(value, 10, M12_C);
}
bool Logo::readAlertTempStage1(bool *value)
{
  return checkCache(value, 11, M13_C);
}
bool Logo::readAlertTempStage2(bool *value)
{
  return checkCache(value, 12, M14_C);
}
bool Logo::readAlertTempStage3(bool *value)
{
  return checkCache(value, 13, M15_C);
}
bool Logo::readAlertSpsOffline(bool *value)
{
  return checkCache(value, 14, M16_C);
}

bool Logo::readMixerValve(bool *value)
{
  return checkCache(value, 15, Q12_C);
}
bool Logo::readCompressorRunning(bool *value)
{
  return checkCache(value, 16, M1_C);
}


bool Logo::checkCache(bool *resultbuffer, int index, int address)
{
  bool value;
  if (address < 25)
  { // it is an input
    value = modbusTCPClient->discreteInputRead(address);
  }
  else
  {
    value = modbusTCPClient->coilRead(address);
  }
  bool changed = (cache[index] != value);
  if (changed)
    cache[index] = value;
  *resultbuffer = value;
  return changed;
}

bool Logo::checkCache(long *resultbuffer, int index, int address)
{
  long value = modbusTCPClient->holdingRegisterRead(address);
  bool changed = (cache[index] != value);
  if (changed)
    cache[index] = value;
  *resultbuffer = value;
  return changed;
}

