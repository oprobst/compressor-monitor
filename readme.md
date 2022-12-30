# TSV Malsch Kompressor Steuerung und Monitor

![Logo Tauchsportverein Malsch](./Dokumentation/Logo%20TSV.png)  

This project contains all sources for the compressor of the Tauchsportverein Malsch e.V.

It consists of:
* Logo!8 Compressor programm
* CAD model of housing for control unit
* LCD Display with Arduino and Ethernet shield to visualize current compressor status
* Documentation folder

## Hardware

### Siemens Logo

| Produkt                    | Product id            | Function                                              |
| -------------------------- | --------------------- | ----------------------------------------------------- |
| LOGO! Power                | 6EP1331-1SH02         | Power supply, reuse of old Logo 6 installation        |
| Logo!8 Logikmodul          | 6ED1052-1HB08-0BA1    | Base module for Siemens Logo                          |
| Siemens LOGO! 8 AM2 RTD    | 6ED1055-1MD00-0BA2    | Temperature measurements room and 3 compressor stages |
| Siemens LOGO! 8 AM2	     | 6ED1055-1MA00-0BA2    | Measurements for preasure air and mix                 |
| Siemens LOGO! 8 DM8 12/24R | 6ED1055-1MB00-0BA2    | Additional digital in- and outputs                    |
| CMK2000                    | 6BK1700-0BA20-0AA0    | Connectivity to home automation                       |
	
### Display and control unit
The LCD Display Application uses an Arduino with ETH shield and LCD Display to connect to Siemens Logo!8 via TCP Modbus and visualizes data.
Purpose is to visualize all runtime parameter of our scuba diving club compressor for the operator.

The System is powered by the 24V DC of the Logo!8. Therefore an additional Step-Down Converter (DC/DC) 24->5V is required. Alternatively, the Ardunio can be powered by its own DC plugin or USB.

These are the components currently in use:
* [UNO R3 ATmega328P Board ATmega16U2 mit USB Kabel Arduino Uno R3 kompatibel](https://eckstein-shop.de/UNOR3ATmega328PBoardATmega16U2mitUSBKabelArduinoUnoR3kompatibel)
* [Waveshare 2.8inch Resistive Touch Display 320x240 TFT Shield ST7789 ](https://eckstein-shop.de/Waveshare28inch320x240TFTHX8347DSPIArduinoResistiveTouchShieldDisplay)
* [LM2596S DC-DC einstellbarer Step-Down Spannungsregler Adjustable Power Supply Modul](https://eckstein-shop.de/LM2596SDC-DCeinstellbarerStep-DownSpannungsreglerAdjustablePowerSupplyModul)
* [SunFounder Ethernet Shield W5100 for Arduino UNO R3 Mega 2560 1280 A057 ](https://smile.amazon.de/gp/product/B00HG82V1A/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1)
* [Anzeigemodul - 3,5" TFT-LCD-Display-Modul 480x320 for Arduino UNO & MEGA 2560](https://smile.amazon.de/gp/product/B07Y5Z6VZB/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1)

## CAD
Folder CAD-Gehäuse contains FreeCAD models for the housing

## Software

### Siemens Logo!8

The program is located in folder Siemens Logo.
Version 3 is the old one for Logo!6 (decommissioned in early 2023).
Version 4 is the current one for Logo!8

### Display
The display controller can be found under Display-Steuerung. The kompressormonitor.ino file uses following support classes:
* DebugSerial: Helper for enabling debug output to Serial Port
* DegreeFont: Just the degree symbol and a 'C'
* Logo: Connectivity to the Logo8 using modbus (acting as client)
* SevenSegFont: Copy of the SevenSegFont of MCUFRIEND_kbv, but with an additional '-' minus character.
* ui: Responsible for rendering the ui.
* data: global shared information object about connectivity status.

## Further documentation

Have a look at folder [Documentation](./Dokumentation/)

Link to this documentation: [https://github.com/oprobst/compressor-monitor/](https://github.com/oprobst/compressor-monitor/)

![QR Code linking this documentation](./Dokumentation/QR-Code-Link-Github.png)
