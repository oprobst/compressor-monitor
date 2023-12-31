/**
 *
 *   Compressor Monitor
 *
 * Access Logo!8 via ModBus to gather a lot of information to display.
 * 
 * 
 */

// Button to allow switching different views.
#define PIN_BUTTON 53

#include <WString.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

#include "DebugSerial.h"
#include "Data.h"
#include "ui.h"
#include "Logo.h"

// configure ethernet setting here:
//  Some random MAC Address (since there is no one on the board)
byte mac[] = {0xAA, 0x12, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192, 168, 1, 35};
byte dns[] = {192, 168, 1, 1};
byte gateway[] = {192, 168, 1, 1};
byte serverIp[] = {192, 168, 1, 100};
int serverPort = 502;

// global shared data object contains all measurements
Data data; 

// Access interface to the Siemens Logo
Logo logo;

UI ui;
EthernetClient client;
ModbusTCPClient modbusTCPClient(client);

// local buffer for values received from sps
bool contactBuf;
long valBuf;
// filltype active last loop
int filltypeLastLoop;

// forward declarations
// initialize the ethernet adapter. Trying DHCP, then fallback to defaults
void initEthernet();
bool checkForAlerts();
void reconnect ();

/**
 * Setup
 *
 */
void setup()
{

#if DEBUG

  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_BUTTON, HIGH);

  Serial.begin(SERIALSPEED);
  while (!Serial)
  {
    ;
  }
#endif

  SPRINTLN("Compressor Monitor application starting");
  ui.initScreen();
  initEthernet();
  delay(500);
  while (!logo.connect(&modbusTCPClient))
  {
    data.modbusConnected = false;
    ui.renderSystemInfo();
    delay(3000);
  } 
  modbusTCPClient.setTimeout(1000);
  delay(200);
  data.modbusConnected = true;

  /*
    for (int i = 8257; i<8300; i++){
      SPRINT(i);
      SPRINT(": ");
      SPRINT(modbusTCPClient.inputRegisterRead(i));
      SPRINT(" | ")
      SPRINT(modbusTCPClient.discreteInputRead(i));
      SPRINT(" | ")
      SPRINT(modbusTCPClient.holdingRegisterRead(i));
      SPRINT(" | ")
      SPRINTLN(modbusTCPClient.coilRead(i));
    }
  */

  // before starting, check if sps in in run mode:
  if (logo.readAlertSpsOffline(&contactBuf))
  {
    if (!contactBuf)
      ui.renderAlert(Alert::SPS_OFFLINE);
  }
}

/**
 * Main loop
 */

void loop()
{
  if (!digitalRead(PIN_BUTTON))
  {
    if (ui.activeView == ACTIVE_VIEW_OVERVIEW)
    {
      ui.activeView = ACTIVE_VIEW_SYSTEMINFO;
      SPRINTLN("User select System info");
    }
    else
    {
      ui.activeView = ACTIVE_VIEW_OVERVIEW;
      SPRINTLN("User select Overview ");
    }
  }
  logo.resetCache();
  delay(250);

  if (ui.activeView == ACTIVE_VIEW_OVERVIEW)
    ui.renderOverviewScreen();

 
  SPRINTLN("Check for Overview");
  while (digitalRead(PIN_BUTTON) && ui.activeView == ACTIVE_VIEW_OVERVIEW )
  {
    // first check for alerts of any kind...
    if (modbusTCPClient.connected() && checkForAlerts())
    {
      ui.activeView = ACTIVE_VIEW_ALERT;
      break;
    }

    // check if we're in air, mix or maintenance mode
    logo.readMixerValve(&contactBuf);
    if (contactBuf)
    {
      ui.fillType = FILLTYPE_MIX;
    }
    else
    {
      ui.fillType = FILLTYPE_AIR;
    }
    // overwrite former result by maintenance if active
    logo.readMaintenanceSwitch(&contactBuf);
    if (contactBuf)
      ui.fillType = FILLTYPE_MAINTENANCE;

    if (filltypeLastLoop != ui.fillType)
    {
      // filltype changed, render screen
      SPRINT("Switching to filltype ");
      SPRINTLN(ui.fillType);
      filltypeLastLoop = ui.fillType;
      logo.resetCache();
      ui.renderOverviewScreen();
    }

    // Finally, check for all measurements
    if (logo.readHeater(&contactBuf))
      ui.showHeating(contactBuf);
    if (logo.readSummerSwitch(&contactBuf))
      ui.showSeason(contactBuf);
    if (logo.readRollershutter(&contactBuf))
      ui.showRollershutter(contactBuf);
    if (logo.readVentilationInside(&contactBuf))
      ui.showVentInside(contactBuf);
    if (logo.readVentilationOutside(&contactBuf))
      ui.showVentOutside(contactBuf);
    
      ui.showFilterStatus(true); //will be overwritten by alert

    if (logo.readRoomTemp(&valBuf))
      ui.showRoomTemp(valBuf);
    if (logo.readCompressorStage1Temp(&valBuf))
      ui.showCompressorStage1Temp(valBuf);
    if (logo.readCompressorStage2Temp(&valBuf))
      ui.showCompressorStage2Temp(valBuf);
    if (logo.readCompressorStage3Temp(&valBuf))
      ui.showCompressorStage3Temp(valBuf);
    if ((ui.fillType == FILLTYPE_AIR || ui.fillType == FILLTYPE_MAINTENANCE) && logo.readPressureAir(&valBuf))
      ui.showPressure(valBuf);
    if (ui.fillType == FILLTYPE_MIX && logo.readPressureMix(&valBuf))
      ui.showPressure(valBuf);
    if (logo.readCompressorRunning(&contactBuf))
    {
      if (contactBuf)
      {
        ui.renderCompressor(COMPRESSOR_STATUS_ON);
      }
      else
      {
        ui.renderCompressor(COMPRESSOR_STATUS_OFF);
      }
    }

    delay(333); // not too much flicker...
  }

//Alert occurred? Show failure screen
  SPRINTLN("Check for Alert View");
  while (ui.activeView == ACTIVE_VIEW_ALERT && modbusTCPClient.connected())
  {
    logo.resetCache();
    if (!checkForAlerts())
      ui.activeView = ACTIVE_VIEW_OVERVIEW;
    delay(2000);
  }

// Modbus Connectivity issues, show system info and try to reconnect.
  if (!modbusTCPClient.connected())
  {
    data.modbusConnected = false;
    ui.activeView = ACTIVE_VIEW_SYSTEMINFO;
  }
  if (ui.activeView == ACTIVE_VIEW_SYSTEMINFO) ui.renderSystemInfo();
  SPRINTLN("Check for System Info");
  while (ui.activeView == ACTIVE_VIEW_SYSTEMINFO && digitalRead(PIN_BUTTON))
  {
    if (!modbusTCPClient.connected()) {
      reconnect();
      data.modbusConnected = true;
      ui.renderSystemInfo();
      delay (1000);
      ui.activeView = ACTIVE_VIEW_OVERVIEW;
    }     
  }
}



bool checkForAlerts()
{

  bool alertActive = false;
  if (logo.readAlertFilterwechsel(&contactBuf))
  {
    if (contactBuf)
    {
      alertActive = true;
      ui.renderAlert(Alert::FILTERWECHSEL);
    }
  }

  if (logo.readAlertKompressorstoerung(&contactBuf))
  {
    if (contactBuf)
    {
      alertActive = true;
      ui.renderAlert(Alert::COMPRESSOR_ERR);
    }
  }

  if (logo.readAlertEmergencyStop(&contactBuf))
  {
    if (contactBuf)
    {
      alertActive = true;
      ui.renderAlert(Alert::EMERGENCY_STOP);
    }
  }

  logo.readAlertSpsOffline(&contactBuf);
  if (!contactBuf)
  {
    alertActive = true;
    ui.renderAlert(Alert::SPS_OFFLINE);
    reconnect();
  }

  if (logo.readAlertTempRoom(&contactBuf))
  {
    if (contactBuf)
    {
      alertActive = true;
      ui.renderAlert(Alert::TEMPERATURE_ROOM);
    }
  }

  if (logo.readAlertTempStage1(&contactBuf))
  {
    if (contactBuf)
    {
      alertActive = true;
      ui.renderAlert(Alert::TEMP_STAGE1);
    }
  }

  if (logo.readAlertTempStage2(&contactBuf))
  {
    if (contactBuf)
    {
      alertActive = true;
      ui.renderAlert(Alert::TEMP_STAGE2);
    }
  }

  if (logo.readAlertTempStage3(&contactBuf))
  {
    if (contactBuf)
    {
      alertActive = true;
      ui.renderAlert(Alert::TEMP_STAGE3);
    }
  }
  return alertActive;
}
/**
 * Init ethernet
 */
void initEthernet()
{
  SPRINT("Get IP via DHCP for mac...");
  if (Ethernet.begin(mac))
  {
    SPRINTLN(" [SUCCESS]");
    data.dhcpSuccess = true;
  }
  else
  {
    SPRINTLN(" [FAILED]");
    data.dhcpSuccess = false;
    SPRINTLN("Unable do acquire IP via DHCP, fallback to static IP");
    Ethernet.begin(mac, ip, dns, gateway);
  }

  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    SPRINTLN("Ethernet shield was not found.");
    data.wifiShieldType = "NONE";
  }
  else if (Ethernet.hardwareStatus() == EthernetW5100)
  {
    SPRINTLN("W5100 Ethernet controller detected.");
    data.wifiShieldType = "W5100";
  }
  else if (Ethernet.hardwareStatus() == EthernetW5200)
  {
    SPRINTLN("W5200 Ethernet controller detected.");
    data.wifiShieldType = "W5200";
  }
  else if (Ethernet.hardwareStatus() == EthernetW5500)
  {
    SPRINTLN("W5500 Ethernet controller detected.");
    data.wifiShieldType = "W5500";
  }
  SPRINT("IP Address: ");
  SPRINTLN(Ethernet.localIP());
  SPRINT("DNS Server: ");
  SPRINTLN(Ethernet.dnsServerIP());
  SPRINT("Gateway   : ");
  SPRINTLN(Ethernet.gatewayIP());
  SPRINT("SubnetMask: ");
  SPRINTLN(Ethernet.subnetMask());
  client.setTimeout(500);
  client.setConnectionTimeout(500);


}

void reconnect (){  
    SPRINTLN("Modbus disconnected, trying to reconnect");
    while (!logo.connect(&modbusTCPClient))
    {
      SPRINT(".");
      delay(1000);
    }
  
}