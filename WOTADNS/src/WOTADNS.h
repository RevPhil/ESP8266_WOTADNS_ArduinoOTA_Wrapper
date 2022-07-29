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
	
	int  beginWiFi(const char* ssid,const char* password, const char* hostname = WiFi.hostname().c_str());
	// Starts up a Wi-fi connection to the specified SSID. 'hostname' is optional but defaults to the
	// ESP device 'hostname'
	
	int  beginWiFiDNS(const char* ssid, const char* password, const char* hostname);
	// Starts up a Wi-fi connection with mDNS capabilies. 'hostname' is mandatory as
	// this will form the Network Name of the device for 'ping' etc.
	
	int  beginWiFiOTA(const char* ssid, const char* password, const char* hostname);
	// Starts up a Wi-fi connection with ArduinoOTA enabled. 'hostname' is mandatory as
	// this will form the Network Name of the device for 'ping', OTA uploading etc.
	
	void setOtaPort(int port);
	// Sets the OTA port which defaults to 8266.
	
	void setOtaPassword(const char* password);
	// Sets the OTA password which defaults to NULL.
	
	void ledON(uint8_t pin);
	// If called, the LED on the given pin will flash during Wi-fi connectiomn attemnpts.
	// LED_BUILTIN is suggested unless there is no built-in LED
	
	void setConnectAttempts(uint8_t num);	// defualt is 15
	// Sets the number of Wi-fi connection attempts before giving up and returning an error.
	
	void printDetails(void);
	// Prints out the Wi-fi connection details on the Serial port.
	
	void refresh(void);
	// Updates the OTA and mDNS libraries.
	
	String pad(String str1, uint8_t len);
	// Adds leading spaces to String 'str1' length = 'len'
	
private:
	void beginOTA(void);
	void ledOFF(void);
	void sigLedOn(void);
	void sigLedOff(void);
	// variables
	uint8_t connectAttempts = 15;
	int8_t ledPin = -1;
	bool ledIsON = false;	// true = ON, false = OFF
	bool modeIsOTA = false;
	bool modeIsDNS = false;
};

#endif
