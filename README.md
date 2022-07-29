# ESP8266_WOTADNS_ArduinoOTA_Wrapper
 A wrapper library to simplify Wi-fi, Over The Air and mDNS implementation...

Although the implementation of both ArduinoOTA and ESP8266mDNS is fairly straightforward when using the ESP8266 Core within the Arduino IDE, a lot of space is taken up with code in sketches.

WOTADNS is the end result and it makes life a little easier and reduces clutter in sketches.

Details of the various methods of the WOTADNS class are given in 'WOTADNS.h'

Two macros are included:

**DISABLE_WIFI**

This macro allows the ESP8266 to be used without Wi-fi

**SERIAL_BEGIN(baudrate)**

Starts up the Serial port properly (in my opinion)