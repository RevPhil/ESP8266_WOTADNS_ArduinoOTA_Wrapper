#ifndef _WOTADNSCLASS_H_
#define _WOTADNSCLASS_H_

/*
	Wi-fi, Over The Air and mDNS Wrapper
	A simple helper class for Wi-Fi, ArduinoOTA and mDNS connection
	functions.
	(C) 2022 Reverend Phil Morris
*/

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

// macros to control the onboard LED
#ifdef ESP8266
#define WOTADNS_LED_ON digitalWrite(ledPin,LOW);
#define WOTADNS_LED_OFF digitalWrite(ledPin,HIGH);
#else
#define WOTADNS_LED_ON digitalWrite(ledPin,HIGH);
#define WOTADNS_LED_OFF digitalWrite(ledPin,LOW);
#endif

// macro to disable Wi-Fi
#define DISABLE_WIFI WiFi.disconnect();\
  WiFi.mode(WIFI_OFF);\
  WiFi.forceSleepBegin();
  
// macro to start the Serial interface properly
#define SERIAL_BEGIN(x) Serial.begin(x);\
  while(!Serial) delay(100);\
  delay(500);

class WOTADNS {
public:
	// methods
	void setConnectAttempts(uint8_t num);	// defualt is 15
	void setOtaPort(int port);
	void setOtaPassword(const char* password);
	int  beginWiFiDNS(const char* ssid, const char* password, const char* hostname);
	int  beginWiFiOTA(const char* ssid, const char* password, const char* hostname);
	int  beginWiFi(const char* ssid,const char* password, const char* hostname = WiFi.hostname().c_str());	
	void ledON(uint8_t pin);
	void ledOFF(void);
	void printDetails(void);
	void refresh(void);
	String pad(String str1, uint8_t len);
	String pad(String str1, String str2, uint8_t len);
	
private:
	void beginOTA(void);
	// variables
	uint8_t connectAttempts = 15;
	int8_t ledPin = -1;
	bool ledIsON = false;	// true = ON, false = OFF
	bool modeIsOTA = false;
	bool modeIsDNS = false;
};

#endif
