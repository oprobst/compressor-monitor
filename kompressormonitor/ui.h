#ifndef UI_H_
#define UI_H_


#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include <Fonts/FreeSerif24pt7b.h > 
#include <Fonts/FreeSans12pt7b.h >
#include <Fonts/FreeMono9pt7b.h>

#include <FreeDefaultFonts.h>

#include <FreeSevenSegNumFontPlusPlus.h>
#include "DebugSerial.h"
#include "SevenSegFont.h"
#include "DegreeFont.h"

#include <Ethernet.h>
#include "Data.h"

extern Data data; 

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x8410

#define FILLTYPE_AIR 0
#define FILLTYPE_MIX 1
#define FILLTYPE_MAINTENANCE 2

#define ACTIVE_VIEW_OVERVIEW 0
#define ACTIVE_VIEW_SYSTEMINFO 1
#define ACTIVE_VIEW_ALERT 2

#define COMPRESSOR_STATUS_OFF 0
#define COMPRESSOR_STATUS_ON 1
#define COMPRESSOR_STATUS_ALERT 2
#define COMPRESSOR_STATUS_MAINTENANCE 3

enum Alert {
  FILTERWECHSEL, TEMPERATURE_ROOM, TEMP_STAGE1, TEMP_STAGE2, TEMP_STAGE3,EMERGENCY_STOP,COMPRESSOR_ERR,SPS_OFFLINE
};

class UI {

private:
MCUFRIEND_kbv tft;
uint16_t color;
char* ip2CharArray(IPAddress ip);
char* mac2CharArray(byte [6]);
void showTemperatureOnLocation(float temp, int location);

public:
int activeView = ACTIVE_VIEW_OVERVIEW;
int fillType = FILLTYPE_AIR;

// Show initial startup screen
void initScreen ();

// Show startup information and Ethernet connectivity
void renderSystemInfo ();

void renderOverviewScreen ();

void renderAlertScreen ();

void renderAlert (Alert alert);


void showPressure(long f);

void showRoomTemp (float f);
void showCompressorStage1Temp (float f);
void showCompressorStage2Temp (float f);
void showCompressorStage3Temp (float f);

void showFilterStatus (bool isGood);
void showVentOutside (bool isRunning);
void showVentInside (bool isRunning);
void showHeating (bool isRunning);
void showSeason (bool isWinter);
void showRollershutter (bool isOpen);


/**
 * Select status from 
 * COMPRESSOR_STATUS_OFF 0
 * COMPRESSOR_STATUS_ON 1
 * COMPRESSOR_STATUS_ALERT 2
 * COMPRESSOR_STATUS_MAINTENANCE 3
 */
void renderCompressor(int status);

};
#endif /* UI_H_ */
