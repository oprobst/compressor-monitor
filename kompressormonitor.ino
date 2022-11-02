#include <SPI.h>
#include <Ethernet.h>

//Some random MAC Address (since there is no one on the board)
byte mac[] = {0xAA, 0x12, 0xBE, 0xEF, 0xFE, 0xED};

#define LAN_NO_ERROR 0
#define LAN_NO_ETH 1
#define LAN_NO_DHCP 2

// Errorcode used to visualize network failures
int lanErrorCode = LAN_NO_ERROR;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("\n\n_________________________________________");
  Serial.println("Compressor Monitor application starting");

  if (Ethernet.begin(mac)) {
	  Serial.println("DHCP successful" );
  } else {
	  Serial.println("Unable do acquire IP via DHCP" );
	  lanErrorCode = LAN_NO_DHCP;
  }

  //
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.");
    lanErrorCode = LAN_NO_ETH;
  }
  else if (Ethernet.hardwareStatus() == EthernetW5100) {
    Serial.println("W5100 Ethernet controller detected.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5200) {
    Serial.println("W5200 Ethernet controller detected.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5500) {
    Serial.println("W5500 Ethernet controller detected.");
  }

  // Show link status, if hardware allows (W5200 & W5500 only):
  if (Ethernet.hardwareStatus() == EthernetW5200 || Ethernet.hardwareStatus() == EthernetW5500) {
	  if (Ethernet.linkStatus() == Unknown) {
		Serial.println("Link status unknown. Link status detection is only available with W5200 and W5500.");
	  }
	  else if (Ethernet.linkStatus() == LinkON) {
		Serial.println("Link status: On");
	  }
	  else if (Ethernet.linkStatus() == LinkOFF) {
		Serial.println("Link status: Off");
	  }
  }

  Serial.print ("IP Address: ");
  Serial.println(Ethernet.localIP());
  Serial.print ("DNS Server: ");
  Serial.println(Ethernet.dnsServerIP());
  Serial.print ("Gateway   : ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print ("SubnetMask: ");
  Serial.println(Ethernet.subnetMask());

}

void loop () {}
