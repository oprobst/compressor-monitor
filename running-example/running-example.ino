// **********************************************
// *
// * Display 480x320
// *

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin


// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x8410


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
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include "Adafruit_GFX.h"   // Core graphics library
#include <MCUFRIEND_kbv.h>  // Hardware-specific library
#include <Fonts/FreeSans12pt7b.h>
#include <FreeDefaultFonts.h>

#include <ArduinoRS485.h>  // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>




//Some random MAC Address (since there is no one on the board)
byte mac[] = { 0xAA, 0x12, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 4, 99 };
byte dns[] = { 192, 168, 4, 7 };
byte gateway[] = { 192, 168, 4, 8 };
byte ipServer[] = { 192, 168, 4, 100 };
int modServerPort = 503; 

#define LAN_NO_ERROR 0
#define LAN_NO_ETH 1
#define LAN_NO_DHCP 2
#define LAN_NO_CABLE 3
#define LAN_NO_CONNECTION 4

// Errorcode used to visualize network failures
int lanErrorCode = LAN_NO_ERROR;

MCUFRIEND_kbv tft;

EthernetClient client;
ModbusTCPClient modbusTCPClient(client);

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x8410



void initScreen (){
   Serial.println("Initializing Display...");
	uint16_t id = tft.readID();
  if (id != 0){
    Serial.println(" [SUCCESS]");
    Serial.print("ID = 0x");
	  Serial.println(id, HEX);
    if (id == 0xD3D3) id = 0x9481; //force ID if write-only display
  } else {
    Serial.println(" [FAILED]");    
  }

	tft.begin(id);
	tft.setRotation(1);
	tft.fillScreen(BLACK);

}

char* ip2CharArray(IPAddress ip) {
	static char a[16];
	sprintf(a, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	return a;
}

void print(int x, int y, int sz, int color, const GFXfont *f, const char *msg)
{
	int16_t x1, y1;
	uint16_t wid, ht;
	tft.setFont(f);
	tft.setCursor(x, y);
	tft.setTextColor(color);
	tft.setTextSize(sz);
	tft.print(msg);
}

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) { ; }
  Serial.println("\n\n_________________________________________");
  Serial.println("Compressor Monitor application starting");
  
 
  Serial.print("Get IP via DHCP...");
  if (Ethernet.begin(mac)) {
    Serial.println(" [SUCCESS]");
  } else {
    Serial.println(" [FAILED]");
    lanErrorCode = LAN_NO_DHCP;
    Serial.println("Unable do acquire IP via DHCP, fallback to static IP");
    Ethernet.begin(mac, ip, dns, gateway);
  }
    
	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
		Serial.println("Ethernet shield was not found.");
		lanErrorCode = LAN_NO_ETH;
	} else if (Ethernet.hardwareStatus() == EthernetW5100) {
		Serial.println("W5100 Ethernet controller detected.");
	} else if (Ethernet.hardwareStatus() == EthernetW5200) {
		Serial.println("W5200 Ethernet controller detected.");
	} else if (Ethernet.hardwareStatus() == EthernetW5500) {
		Serial.println("W5500 Ethernet controller detected.");
	}
  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
  Serial.print("DNS Server: ");
  Serial.println(Ethernet.dnsServerIP());
  Serial.print("Gateway   : ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("SubnetMask: ");
  Serial.println(Ethernet.subnetMask()); 
}

long c1 = 0;
long c2 = 0;
long c3 = 0;
long c1o = -2;
long c2o = -2;
long c3o = -2;


void loop() {
  while (!modbusTCPClient.connected()){
    Serial.print("Trying to connect to Modbus Server at ");
    Serial.print(IPAddress(ipServer));
    Serial.print(":");
    Serial.print(modServerPort);
    Serial.print("...");
    if (!modbusTCPClient.begin(IPAddress(ipServer), modServerPort)) {
      Serial.println (" [FAILED]");
      Serial.println ("Retry in 3 seconds");
      delay (3000);
    } else{ 
      Serial.println (" [SUCCESS]");
    }
  }
  int i = 0;
  while (modbusTCPClient.connected()) {

    c1 = modbusTCPClient.inputRegisterRead(AI1_IR);
    if (c1o != c1) {
      Serial.print("c1: ");
      Serial.println(c1);
      c1o = c1;
    }
   //Serial.print (i);
    c2 = modbusTCPClient.holdingRegisterRead(AM1_HR);
    if (c2o != c2) {
   //   Serial.print (i);
      Serial.print("c2: ");
      Serial.println(c2);
      char result[8];
      tft.fillRect(50,10,300,300,BLACK);
      print (50,100,3, GREEN, &FreeSans12pt7b, dtostrf((c2/10.0f), 6, 1, result));

      c2o = c2;
    }

    c3 = modbusTCPClient.discreteInputRead(I1_DI);
    if (c3o != c3) {
      Serial.print("c3: ");
      Serial.println(c3);
      c3o = c3;
    }
   

    print(400, 310, 1, GREY, NULL , ip2CharArray(Ethernet.localIP()));

    delay(333);
  }
}