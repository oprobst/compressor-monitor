

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

#define DEBUG 1
#if DEBUG
#define SPRINT(s)    \
  {                  \
    Serial.print(s); \
  }
#define SPRINTLN(s)    \
  {                    \
    Serial.println(s); \
  }
#else
#define SPRINT(s)
#define SPRINTLN(s)
#endif
#define SERIALSPEED 9600

#include <WString.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

#include "Data.h"
#include "ui.h"

// global shared data object contains all measurements
Data data;
// global shared configuration object
//Config config;

// mac = {0xAA, 0x12, 0xBE, 0xEF, 0xFE, 0xED};
// ip = {192, 168, 4, 99};
// dns = {192, 168, 4, 7};
// gateway = {192, 168, 4, 8};
// serverIp = {192, 168, 4, 100};
// serverPort = 503;

// Some random MAC Address (since there is no one on the board)
  byte mac[] = { 0xAA, 0x12, 0xBE, 0xEF, 0xFE, 0xED };
  byte ip[] = { 192, 168, 4, 99 };
  byte dns[] = { 192, 168, 4, 7 };
  byte gateway[] = { 192, 168, 4, 8 };
  byte serverIp[] = { 192, 168, 4, 100 };
  int serverPort = 503;

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



void setup()
{

#if DEBUG
  Serial.begin(SERIALSPEED);
  while (!Serial)
  {
    ;
  }
#endif
  SPRINTLN("Compressor Monitor application starting");

  ui.initScreen();
  initEthernet();
  delay (1000);
}

long c1 = 0;
long c2 = 0;
long c3 = 0;
long c1o = -2;
long c2o = -2;
long c3o = -2;

void loop()
{
  
  while (!modbusTCPClient.connected())
  {
    ui.renderSystemInfo(); 
    SPRINT("Trying to connect to Modbus Server at ");
    SPRINT(IPAddress(serverIp));
    SPRINT(":");
    SPRINT(serverPort);
    SPRINT("...");
    if (!modbusTCPClient.begin(IPAddress(serverIp), serverPort))
    {      
      data.connected = false;
      SPRINTLN(" [FAILED]");
      SPRINTLN("Retry in 3 seconds");
      ui.renderSystemInfo();          
    }
    else
    {
      SPRINTLN(" [SUCCESS]");
      data.connected = true;
    }
  }

  int i = 0;
  ui.renderOverviewScreen();
  while (modbusTCPClient.connected())
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
}

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


  SPRINT("Trying to connect to Modbus Server at ");
  SPRINT(IPAddress(serverIp));
  SPRINT(":");
  SPRINT(serverPort);
  SPRINT("...");
}