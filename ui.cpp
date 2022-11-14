#include "ui.h"
#include "DebugSerial.h"

extern byte mac[];
extern byte ip[];
extern byte dns[];
extern byte gateway[];
extern byte serverIp[];
extern int serverPort;

void UI::print(int x, int y, int sz, int iconColor, const char *msg)
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

	color = BLACK;
	tft.begin(id);
	// tft.cp437(true);
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
	// tft.fillScreen(BLACK);
}

void UI::renderOverviewScreen()
{
	tft.setTextSize(1);
	tft.setFont(NULL);
	tft.fillScreen(BLACK);
	tft.setFont(&FreeSans12pt7b);

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
	tft.print("323");
	// in bar
	tft.setFont(&FreeSans12pt7b);
	tft.setTextSize(2);
	tft.print("   bar");
	tft.drawLine(0, 150, 480, 150, color);

	// show first temperature block
	tft.drawLine(0, 220, 480, 220, color);
	tft.drawLine(240, 150, 240, 220, color);
	showTemperatureOnLocation(-274, 0);
	showTemperatureOnLocation(-274, 1);
	// in GRad

	// show seconnd temperature block
	tft.drawLine(0, 290, 480, 290, color);
	tft.drawLine(240, 220, 240, 290, color);
	showTemperatureOnLocation(-274, 2);
	showTemperatureOnLocation(-274, 3);
}

void UI::renderSystemInfo()
{
	tft.fillScreen(BLACK);
	tft.setFont(NULL);
	tft.setCursor(0, 0);
	tft.setTextColor(WHITE, BLACK);
	tft.setTextSize(2);
	tft.print("System Information");
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
	// tft.setCursor(0, 200);
	// tft.setTextColor(WHITE);
	// tft.print("Uptime     : ");
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

void UI::showTemperatureOnLocation(float temp, int location)
{
	tft.setTextSize(1);
	switch (location)
	{
	case 0:
		tft.setCursor(20, 210);
		tft.fillRect(1, 151, 238, 69, BLACK);
		break;
	case 1:
		tft.setCursor(260, 210);
		tft.fillRect(241, 151, 238, 69, BLACK);
		break;
	case 2:
		tft.setCursor(20, 280);
		tft.fillRect(1, 221, 238, 69, BLACK);
		break;
	default:
		tft.setCursor(260, 280);
		tft.fillRect(241, 221, 238, 69, BLACK);
		break;
	}
	tft.setFont(&FreeMono18pt7b);
	if (temp > -273)
	{
		tft.print(temp, 1);
	}
	else
	{
		tft.print("---");
	}
	// in GRad
	tft.setFont(&FreeSans12pt7b);
	tft.setTextSize(1);
	// tft.print (0x00BA);
	tft.print("   'C");
}
void UI::showRoomTemp(float f)
{
	showTemperatureOnLocation(f, 0);
}

void UI::showCompressorStage1Temp(float f)
{
	showTemperatureOnLocation(f, 1);
}

void UI::showCompressorStage2Temp(float f)
{
	showTemperatureOnLocation(f, 2);
}
void UI::showCompressorStage3Temp(float f)
{
	showTemperatureOnLocation(f, 3);
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

void UI::renderCompressor(int status)
{
	uint16_t iconColor = BLACK;
	String text = "";

	switch (status)
	{
	case COMPRESSOR_STATUS_ON:
		iconColor = GREEN;
		text = "AN";
		break;
	case COMPRESSOR_STATUS_OFF:
		iconColor = GREY;
		text = "AUS";
		break;
	case COMPRESSOR_STATUS_MAINTENANCE:
		iconColor = YELLOW;
		text = "WART.";
		break;
	case COMPRESSOR_STATUS_ALERT:
		iconColor = RED;
		text = "ERR";
		break;
	default:
		iconColor = BLACK;
		text = "UNKNOWN";
	}
	int offX = 400;
	int offY = 50;
	tft.fillCircle(offX + 30, offY + 30, 30, BLACK);
	tft.drawCircle(offX + 30, offY + 30, 28, iconColor);
	tft.drawCircle(offX + 30, offY + 30, 29, iconColor);
	tft.drawCircle(offX + 30, offY + 30, 30, iconColor);

	tft.drawLine(offX + 7, offY + 10, offX + 58, offY + 20, iconColor);
	tft.drawLine(offX + 7, offY + 50, offX + 58, offY + 40, iconColor);

	tft.drawLine(offX + 7, offY + 11, offX + 58, offY + 21, iconColor);
	tft.drawLine(offX + 7, offY + 49, offX + 58, offY + 39, iconColor);

	tft.drawLine(offX + 25, offY + 31, offX + 40, offY + 31, iconColor);
	tft.drawLine(offX + 25, offY + 30, offX + 40, offY + 30, iconColor);
	tft.drawLine(offX + 25, offY + 29, offX + 40, offY + 29, iconColor);

	tft.drawLine(offX + 41, offY + 25, offX + 41, offY + 35, iconColor);
	tft.drawLine(offX + 40, offY + 25, offX + 40, offY + 35, iconColor);
	tft.drawLine(offX + 39, offY + 25, offX + 39, offY + 35, iconColor);

	tft.setTextSize(1);
	tft.setTextColor(iconColor, BLACK);
	tft.setCursor(offX, offY + 85);
	tft.print(text);
	tft.setTextColor(color, BLACK);
}