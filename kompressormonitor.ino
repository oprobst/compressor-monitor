

// **********************************************
// *
// * Display 480x320
// *

#define LCD_CS A3    // Chip Select goes to Analog 3
#define LCD_CD A2    // Command/Data goes to Analog 2
#define LCD_WR A1    // LCD Write goes to Analog 1
#define LCD_RD A0    // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
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

#define AI1_IR 1
#define AI2_IR 2
#define AI3_IR 3
#define AI4_IR 4
#define AI5_IR 5
#define AI6_IR 6
#define AI7_IR 7
#define AI8_IR 8

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

#define PIN_BUTTON 53

// global shared data object contains all measurements
Data data;

// Access interface to the Siemens Logo
Logo logo;

// Some random MAC Address (since there is no one on the board)
byte mac[] = {0xAA, 0x12, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192, 168, 4, 99};
byte dns[] = {192, 168, 4, 7};
byte gateway[] = {192, 168, 4, 8};
byte serverIp[] = {192, 168, 4, 100};
int serverPort = 502;

#define LAN_NO_ERROR 0
#define LAN_NO_ETH 1
#define LAN_NO_DHCP 2
#define LAN_NO_CABLE 3
#define LAN_NO_CONNECTION 4

// Errorcode used to visualize network failures
int lanErrorCode = LAN_NO_ERROR;

UI ui;
EthernetClient client;
ModbusTCPClient modbusTCPClient(client);

// forward declarations
// initialize the ethernet adapter. Trying DHCP, then fallback to defaults
void initEthernet();

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
    ui.renderSystemInfo();
    delay(3000);
  }
  delay(200);
}

/**
 * Main loop
 */
float valBuf;
bool contactBuf;

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

  if (!modbusTCPClient.connected())
  {
    SPRINTLN("Modbus disconnected, trying to reconnect");
    while (!logo.connect(&modbusTCPClient))
    {
      SPRINT(".");
      delay(100);
    }
  }

  while (digitalRead(PIN_BUTTON) && ui.activeView == ACTIVE_VIEW_OVERVIEW && modbusTCPClient.connected())
  {
    delay(250);

    if (logo.readRoomTemp(&valBuf))
      ui.showRoomTemp(valBuf);
    if (logo.readCompressorStage1Temp(&valBuf))
      ui.showCompressorStage1Temp(valBuf);
    if (logo.readCompressorStage2Temp(&valBuf))
      ui.showCompressorStage2Temp(valBuf);
    if (logo.readCompressorStage3Temp(&valBuf))
      ui.showCompressorStage3Temp(valBuf);

    if (logo.readEmergencyOffSwitch(&contactBuf))
      ui.showEmergencyOffSwitch(contactBuf);

    if (logo.readPressureAir(&valBuf))
      ui.showPressureAir(valBuf);

    if (logo.readMaintenanceSwitch(&contactBuf))
    {
      if (contactBuf)
      {
        logo.resetCache();
        ui.fillType = FILLTYPE_MAINTENANCE;
        ui.renderOverviewScreen();
        ui.renderCompressor(COMPRESSOR_STATUS_MAINTENANCE);
      }
      else
      {
        // todo determine if mixer or air
        logo.resetCache();
        ui.fillType = FILLTYPE_AIR;
        ui.renderOverviewScreen();
        ui.renderCompressor(COMPRESSOR_STATUS_OFF);
      }
    }
  }

  if (ui.activeView == ACTIVE_VIEW_SYSTEMINFO)
    ui.renderSystemInfo();
  while (ui.activeView == ACTIVE_VIEW_SYSTEMINFO && digitalRead(PIN_BUTTON))
  {
    delay(200);
  }
}

/**
 * Init ethernet
 */
void initEthernet()
{
  SPRINT("Get IP via DHCP...");
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
}