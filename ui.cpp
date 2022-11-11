#include "ui.h"
#include <uptime.h>
#include "DebugSerial.h"

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
	// tft.setFont( &FreeSans18pt7b);
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
	tft.setFont(&FreeSerif24pt7b);
	tft.setCursor(120, 130);
	tft.setTextColor(CYAN, BLACK);
	tft.setTextSize(2);

	tft.print("T.S.V.");
	tft.setCursor(110, 220);
	tft.print("Malsch");

	tft.setTextSize(1);
	tft.setFont(NULL);
	tft.fillScreen(BLACK);
}

void UI::renderOverviewScreen()
{ 
	tft.setTextSize(1);
	tft.setFont(NULL);
	tft.fillScreen(BLACK);
	tft.setFont(&FreeSans12pt7b);

	uint16_t color;
	tft.setCursor(384, 24);

	String filltypeName = "";
	switch (fillType)
	{
	case FILLTYPE_AIR:
		color = CYAN;
		filltypeName = "Luft";
		break;
	case FILLTYPE_MIX:
		color = GREEN;
		filltypeName = "Mischer";
		break;
	default: // FILLTYPE_MAINTENANCE
		color = RED;
		filltypeName = "Wartung";
	}

	tft.setTextColor(color);
	// border whole screen
	tft.drawRect(0, 22, 480, 298, color);

	// label mixer/air
	tft.fillRect(380, 0, 100, 30, BLACK);
	tft.drawRoundRect(380, 0, 100, 30, 4, color);
	tft.print(filltypeName);

	// render clock
	tft.setFont(&FreeSans9pt7b);
	tft.setCursor(10, 18);
	tft.print("Kompressor Monitor");

	// show pressure

	tft.setCursor(14, 136);
	tft.setTextSize(2);
	tft.setFont(&FreeSevenSegNumFontPlusPlus);
	tft.print("   ");
	// in bar
	tft.setFont(&FreeSans12pt7b);
	tft.setTextSize(2);
	tft.print("123  bar");
	tft.drawLine(0, 150, 480, 150, color);

	// show temperatures
	tft.drawLine(0, 220, 480, 220, color);
	tft.drawLine(240, 150, 240, 220, color);
	tft.setCursor(20, 210);
	tft.setTextSize(1);
	tft.setFont(&FreeSerif24pt7b);
	tft.print("123");
	// in GRad
	tft.setFont(&FreeSans12pt7b);
	tft.setTextSize(1);
	tft.print("   °C");
	tft.setCursor(260, 210);
	tft.setFont(&FreeSerif24pt7b);
	tft.print("123");
	// in GRad
	tft.setFont(&FreeSans12pt7b);
	tft.setTextSize(1);
	tft.print("   °C");

	tft.drawLine(0, 246, 480, 246, color);
	tft.drawLine(0, 272, 480, 272, color);
	tft.drawLine(0, 298, 480, 298, color);

	// tft.fillScreen(BLACK);

	// void fillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
}

void UI::renderSystemInfo()
{ 
	tft.fillScreen(BLACK);
	tft.setFont(NULL);
	tft.setCursor(0, 0);
	tft.setTextColor(WHITE, BLACK);
	tft.setTextSize(2);
	tft.print("Compressor Monitor");
	// tft.drawLine(0, 15, 480, 15, GREY);
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
	//tft.setCursor(0, 200);
	//tft.setTextColor(WHITE);
	//tft.print("Uptime     : ");
	
}

void UI::updateUptime (){
 	tft.setTextColor(WHITE, BLACK);
	tft.setCursor(150, 200);
	 
	uptime::calculateUptime();
	String uptimeString = "";
	uptimeString+= uptime::getDays();
	uptimeString+="d, ";
	uptimeString+=uptime::getHours();
	uptimeString+=":";
	uptimeString+=uptime::getMinutes();
	uptimeString+=":";
	uptimeString+=uptime::getSeconds();
	uptimeString+=",";
	uptimeString+=uptime::getMilliseconds();
	uptimeString+="            ";
	tft.print (uptimeString);
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

void UI::showRoomTemp(float f)
{
	tft.fillRect(260, 158, 218, 54, BLACK);
	tft.setCursor(260, 210);
	tft.setFont(&FreeSevenSegNumFontPlusPlus);
	tft.print(f, 1);
	// in GRad
	tft.setFont(&FreeSans12pt7b);
	tft.setTextSize(1);
	tft.print("   °C");
}

void UI::showEmergencyOffSwitch(bool f)
{
	if (f)
	{
		tft.setCursor(10, 319);
		tft.fillRect(1, 300, 478, 19, BLACK);
		tft.print("NOT AUS betaetigt!");
	}
	else
	{
		tft.setCursor(10, 318);
		tft.fillRect(1, 300, 478, 19, BLACK);
	}
}