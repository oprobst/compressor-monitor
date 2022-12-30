#include "ui.h"

extern byte mac[];
extern byte ip[];
extern byte dns[];
extern byte gateway[];
extern byte serverIp[];
extern int serverPort;

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

void UI::renderAlert(Alert alert)
{
	tft.setTextSize(1);
	tft.fillScreen(RED);
	tft.setTextColor(WHITE);
	tft.setFont(&FreeSerif24pt7b);
	tft.setCursor(0, 150);
	switch (alert)
	{
	case FILTERWECHSEL:
		tft.print(" Filterwechsel \n notwendig");
		break;
	case TEMPERATURE_ROOM:
		tft.print(" Raumtemperatur \n ausserhalb zul. Bereich");
		break;
	case TEMP_STAGE1:

		tft.print(" Temperatur Stufe 1\n zu hoch");
		break;

	case TEMP_STAGE2:
		tft.print(" Temperatur Stufe 2\n zu hoch");

		break;
	case TEMP_STAGE3:
		tft.print(" Temperatur Stufe 3\n zu hoch");
		break;
	case EMERGENCY_STOP:
		tft.print(" NOT-AUS betaetigt");
		break;
	case COMPRESSOR_ERR:
		tft.print(" Kompressor \n meldet Fehler");
		break;
	case SPS_OFFLINE:
		tft.print(" SPS auf STOP");
		break;
	default:
		tft.print(" Unbekannter Fehler");
	}

	tft.setFont(&FreeMono9pt7b);
	tft.setCursor(4, 300);
	tft.print("Fehler beheben und auf SPS quittieren");
}

void UI::renderOverviewScreen()
{
	tft.setTextSize(1);
	tft.setFont(NULL);
	tft.fillScreen(BLACK);
	tft.setFont(&FreeSans12pt7b);

	String filltypeName = "";
	switch (fillType)
	{
	case FILLTYPE_AIR:
		color = CYAN;
		filltypeName = "Luft";
		renderCompressor(COMPRESSOR_STATUS_OFF);
		break;
	case FILLTYPE_MIX:
		color = GREEN;
		filltypeName = "Mischer";
		renderCompressor(COMPRESSOR_STATUS_OFF);
		break;
	default: // FILLTYPE_MAINTENANCE
		color = RED;
		filltypeName = "Wartung";
		renderCompressor(COMPRESSOR_STATUS_MAINTENANCE);
	}

	tft.setTextColor(color);
	// border whole screen
	tft.drawRect(0, 22, 480, 298, color);

	// label mixer/air
	tft.fillRect(380, 0, 100, 30, BLACK);
	tft.drawRoundRect(380, 0, 100, 30, 4, color);

	tft.setCursor(384, 24);
	tft.print(filltypeName);

	// render clock
	tft.setFont(&FreeMono9pt7b);
	tft.setCursor(10, 18);
	tft.print("Kompressor Monitor TSV Malsch");

	// show pressure
	showPressure(-1.0);

	// show first temperature block
	tft.drawLine(0, 218, 480, 218, color);
	tft.drawLine(240, 150, 240, 218, color);
	showTemperatureOnLocation(-274, 0);
	showTemperatureOnLocation(-274, 1);
	// in GRad

	// show seconnd temperature block
	tft.drawLine(0, 286, 480, 286, color);
	tft.drawLine(240, 218, 240, 286, color);
	showTemperatureOnLocation(-274, 2);
	showTemperatureOnLocation(-274, 3);

	renderCompressor(COMPRESSOR_STATUS_OFF);

	// hier
	tft.setTextColor(WHITE);
	tft.setFont(&FreeMono9pt7b);
	tft.setCursor(2, 300);
	tft.print("Filter:     Vent. Aussen:    Saison:");
	tft.setCursor(2, 311);
	tft.print("Heizung:    Vent. Innen:     Rollladen:");
	tft.setTextColor(color);
}

void UI::showPressure(long f)
{
	tft.fillRect(1, 23, 359, 126, BLACK);
	tft.setCursor(14, 136);
	tft.setTextSize(2);
	tft.setFont(&SevenSegFont);
	if (f > -1)
	{
		tft.print(f);
	}
	else
	{
		tft.print("---");
	}

	// in bar
	tft.setFont(&FreeSans12pt7b);
	tft.setTextSize(2);
	tft.print("   bar");
	tft.drawLine(0, 150, 480, 150, color);
	tft.setTextSize(1);
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
	switch (location)
	{
	case 0:
		tft.setCursor(20, 209);
		tft.fillRect(1, 151, 238, 66, BLACK);
		break;
	case 1:
		tft.setCursor(260, 209);
		tft.fillRect(241, 151, 238, 66, BLACK);
		break;
	case 2:
		tft.setCursor(20, 279);
		tft.fillRect(1, 219, 238, 66, BLACK);
		break;
	default:
		tft.setCursor(260, 279);
		tft.fillRect(241, 219, 238, 66, BLACK);
		break;
	}
	tft.setFont(&SevenSegFont);
	if (temp > -273)
	{
		tft.print(temp / 10.0f, 1);
	}
	else
	{
		tft.print("---");
	}
	// in GRad
	tft.setFont(&DegreeFont);
	tft.setTextSize(1);
	// tft.print (0x00BA);
	tft.print("   oC"); // special font rendering 'o' as degree symbol.
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

void UI::renderCompressor(int status)
{
	uint16_t iconColor = BLACK;
	String text = "";
	tft.setFont(&FreeSans12pt7b);
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

	tft.setTextColor(iconColor, BLACK);
	tft.setCursor(offX, offY + 85);

	tft.fillRect(offX, offY + 65, 478 - offX, 22, BLACK);
	tft.print(text);
	tft.setTextColor(color, BLACK);
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
		tft.setTextColor(GREEN, BLACK);
		tft.print("[SUCCESS]");
	}
	else
	{
		tft.setTextColor(RED, BLACK);
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

	tft.print("\nTCP connected   : ");
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
	tft.print("\nModBus connected: ");
	if (data.modbusConnected)
	{

		tft.setTextColor(GREEN, BLACK);
		tft.print("[SUCCESS]");
	}
	else
	{
		tft.setTextColor(RED, BLACK);
		tft.print("[FAILED] ");
	}
	tft.setTextSize(1);
}

void UI::showFilterStatus(bool isGood)
{
	tft.setFont(&FreeMono9pt7b);
	tft.setCursor(90, 300);
	tft.fillRect(90, 291, 32, 12, BLACK);
	if (isGood)
	{
		tft.setTextColor(GREEN);
		tft.print("GUT");
	}
	else
	{
		tft.setTextColor(RED);
		tft.print("HI");
	}
	tft.setTextColor(color);
}

void UI::showVentOutside(bool isRunning)
{
	tft.setFont(&FreeMono9pt7b);
	tft.fillRect(280, 291, 30, 12, BLACK);
	tft.setCursor(280, 300);

	if (isRunning)
	{
		tft.setTextColor(GREEN);
		tft.print("AN");
	}
	else
	{
		tft.setTextColor(BLUE);
		tft.print("AUS");
	}
	tft.setTextColor(color);
}

void UI::showVentInside(bool isRunning)
{
	tft.setFont(&FreeMono9pt7b);
	tft.fillRect(280, 302, 30, 12, BLACK);
	tft.setCursor(280, 312);
	if (isRunning)
	{
		tft.setTextColor(GREEN);
		tft.print("AN");
	}
	else
	{
		tft.setTextColor(BLUE);
		tft.print("AUS");
	}
	tft.setTextColor(color);
}

void UI::showHeating(bool isRunning)
{
	tft.setFont(&FreeMono9pt7b);
	tft.fillRect(90, 302, 32, 12, BLACK);
	tft.setCursor(90, 312);
	if (isRunning)
	{
		tft.setTextColor(GREEN);
		tft.print("AN");
	}
	else
	{
		tft.setTextColor(BLUE);
		tft.print("AUS");
	}
	tft.setTextColor(color);
}

void UI::showSeason(bool isWinter)
{
	tft.setFont(&FreeMono9pt7b);
	tft.fillRect(440, 291, 32, 12, BLACK);
	tft.setCursor(440, 300);

	if (isWinter)
	{
		tft.setTextColor(CYAN);
		tft.print("Win");
	}
	else
	{
		tft.setTextColor(YELLOW);
		tft.print("Som");
	}
	tft.setTextColor(color);
}

void UI::showRollershutter(bool isOpen)
{
	tft.setFont(&FreeMono9pt7b);
	tft.fillRect(440, 302, 32, 12, BLACK);
	tft.setCursor(440, 312);
	if (isOpen)
	{
		tft.setTextColor(GREEN);
		tft.print("AUF");
	}
	else
	{
		tft.setTextColor(BLUE);
		tft.print("ZU");
	}
	tft.setTextColor(color);
}
