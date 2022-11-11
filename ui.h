#ifndef UI_H_
#define UI_H_


#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include <Fonts/FreeSerif24pt7b.h >
#include <Fonts/FreeSans12pt7b.h >
#include <Fonts/FreeSans9pt7b.h>
#include <FreeDefaultFonts.h>
#include <FreeSevenSegNumFontPlusPlus.h>


#include <Ethernet.h>
#include "Data.h"
#include "Config.h"

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



class UI {

private:
MCUFRIEND_kbv tft;

char* ip2CharArray(IPAddress ip);
char* mac2CharArray(byte [6]);


public:
int activeView = ACTIVE_VIEW_OVERVIEW;
int fillType = FILLTYPE_AIR;

// Show initial startup screen
void initScreen ();

// Show startup information and Ethernet connectivity
void renderSystemInfo ();

void renderOverviewScreen ();

void renderAlertScreen ();

void print (String c);

void println(String c);


void print(int x, int y, int sz, int color, const char *msg);

void printResult (int i);

void showRoomTemp (float f);
void showEmergencyOffSwitch (bool f);

void updateUptime ();

};
#endif /* UI_H_ */
