# Compressor Monitor

Application uses an Arduino with ETH shield and LCD Display to connect to Siemens Logo!8 via TCP Modbus and visualizes data.
Purpose is to visualize all runtime parameter of our scuba diving club compressor for the operator.

## Hardware

The System is powered by the 24V DC of the Logo!8. Therefore an additional Step-Down Converter (DC/DC) 24->5V is required.
It powers the Arduino Uno (or compatible). On top of the Arduino, an Ethernet Shield is located. 

These are the components currently in use:
* [UNO R3 ATmega328P Board ATmega16U2 mit USB Kabel Arduino Uno R3 kompatibel](https://eckstein-shop.de/UNOR3ATmega328PBoardATmega16U2mitUSBKabelArduinoUnoR3kompatibel)
* [Waveshare 2.8inch Resistive Touch Display 320x240 TFT Shield ST7789 ](https://eckstein-shop.de/Waveshare28inch320x240TFTHX8347DSPIArduinoResistiveTouchShieldDisplay)
* [LM2596S DC-DC einstellbarer Step-Down Spannungsregler Adjustable Power Supply Modul](https://eckstein-shop.de/LM2596SDC-DCeinstellbarerStep-DownSpannungsreglerAdjustablePowerSupplyModul)
* [SunFounder Ethernet Shield W5100 for Arduino UNO R3 Mega 2560 1280 A057 ](https://smile.amazon.de/gp/product/B00HG82V1A/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1)
* [Anzeigemodul - 3,5" TFT-LCD-Display-Modul 480x320 for Arduino UNO & MEGA 2560](https://smile.amazon.de/gp/product/B07Y5Z6VZB/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1)

## Software