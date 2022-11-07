#include "ui.h"
#include "uptime.h"

#define DEBUG 0
#if DEBUG
#define SPRINT(s)        \
	{                    \
		Serial.print(s); \
	}
#define SPRINTLN(s)        \
	{                      \
		Serial.println(s); \
	}
#else
#define SPRINT(s)
#define SPRINTLN(s)
#endif

extern byte mac[];
extern byte ip[];
extern byte dns[];
extern byte gateway[];
extern byte serverIp[];
extern int serverPort;

void UI::print(int x, int y, int sz, int color, const char *msg)
{
	int16_t x1, y1;
	uint16_t wid, ht;
	// tft.setFont( &FreeSans12pt7b);
	tft.setCursor(x, y);
	tft.setTextColor(color);
	tft.setTextSize(sz);
	tft.print(msg);
}

void UI::initScreen()
{

	SPRINT("Init Display...");
	uint16_t id = tft.readID();
	if (id != 0)
	{
		SPRINT("ID = 0x");
		if (DEBUG)
		{
			Serial.print(id, HEX);
		}
		SPRINTLN(" [SUCCESS]");

		if (id == 0xD3D3)
			id = 0x9481; // force ID if write-only display
	}
	else
	{
		SPRINTLN(" [FAILED]");
	}

	tft.begin(id);
	tft.setRotation(1);
	tft.fillScreen(BLACK);
//	print(10, 10, 1, BLUE, "Hello\nWorld");
}

void UI::renderSystemInfo()
{
	//tft.fillScreen(BLACK);
	tft.setCursor(0, 0);
	tft.setTextColor(WHITE, BLACK);
	tft.setTextSize(2);
	tft.print("Compressor Monitor");
	//tft.drawLine(0, 15, 480, 15, GREY);
	tft.setCursor(0, 10);
	tft.print("\nDisplay ID : 0x");
	tft.print(tft.readID(), HEX);
	tft.print("\nWiFi Shield: ");
	tft.print(data.wifiShieldType);
	tft.print("\nDHCP       : ");
	if (data.dhcpSuccess)
	{
		tft.setTextColor(GREEN);
		tft.print("[SUCCESS]");
	}
	else
	{
		tft.setTextColor(RED);
		tft.print("[FAILED] ");
	}
	
	tft.setTextColor(WHITE, BLACK);
	tft.print("\nMAC Address: ");
	tft.print(mac2CharArray(mac));
	tft.print("\nIP Address : ");
	tft.print(Ethernet.localIP());
	tft.print("\nDNS Server : ");
	tft.print(Ethernet.dnsServerIP());
	tft.print("\nGateway    : ");
	tft.print(Ethernet.gatewayIP());
	tft.print("\nSubnetMask : ");
	tft.print(Ethernet.subnetMask());
	tft.print("\nServer     : ");
	tft.print(ip2CharArray(serverIp));
	tft.print(":");
	tft.print(serverPort);
	tft.print("\nConnected  : ");
	if (data.connected)
	{
		tft.setTextColor(GREEN, BLACK);
		tft.print("[SUCCESS]");
	}
	else
	{
		tft.setTextColor(RED, BLACK);
		tft.print("[FAILED] ");
	}
	tft.setTextColor(WHITE, BLACK);
	tft.print("\nUptime     : ");

	uptime::calculateUptime();
	tft.print(uptime::getDays());
	tft.print("d, ");
	tft.print(uptime::getHours());
	tft.print(":");
	tft.print(uptime::getMinutes());
	tft.print(":");
	tft.print(uptime::getSeconds());
	tft.print(",");
	tft.print(uptime::getMilliseconds());
}

char *UI::ip2CharArray(IPAddress ip)
{
	static char a[16];
	sprintf(a, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	return a;
}

char *UI::mac2CharArray(byte mac[6])
{
	static char a[32];
	sprintf(a, "%d-%d-%d-%d-%d-%d", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return a;
}

void UI::printResult(int i)
{

	char result[8];
	// tft.fillRect(50,10,300,300,BLACK);
	print(50, 100, 3, GREEN, dtostrf((i / 10.0f), 6, 1, result));
}
