#include <WOTADNS.h>

void WOTADNS::setConnectAttempts(uint8_t num) {
	connectAttempts = num;
}

void WOTADNS::setOtaPort(int port) {
	ArduinoOTA.setPort(port);
}

void WOTADNS::setOtaPassword(const char* password) {
	ArduinoOTA.setPassword(password);
}

int WOTADNS::beginWiFiDNS(const char* ssid, const char* password, const char* hostname) {
	WiFi.hostname(hostname);
	if(beginWiFi(ssid,password) < 0) return -1;
	if (!MDNS.begin(hostname)) return -2;
	modeIsDNS = true;
	return 0;
}

// start both Wi-Fi and OTA, OTA hostname defaults to the Wi-Fi hostname
// if WiFi.hostname(<hostname>) is invoked, the WiFi hostname is used
// as the ArduinoOTA hostname
int WOTADNS::beginWiFiOTA(const char* ssid, const char* password, const char* hostname) {
	WiFi.hostname(hostname);
	if(!beginWiFi(ssid,password) < 0) return -1;
	ArduinoOTA.setHostname(hostname);
	beginOTA();
	modeIsOTA = true;
	return 0;
}

// start the Wi-Fi connection and returns true if successful, false if failed
// this function does not start mDNS
int WOTADNS::beginWiFi(const char* ssid, const char* password,const char* hostname) {
  // start the Wi-Fi connection with optional hostname
	if(strlen(hostname) > 0) WiFi.setHostname(hostname);
    WiFi.begin(ssid, password);
    // now wait for a connection, try 25 times to connect
    bool wifiTimeout = true;
    int8_t connectLoop = connectAttempts;
    while (connectLoop-- && wifiTimeout) {
      if (WiFi.status() == WL_CONNECTED) {
        wifiTimeout = false;
		if(ledIsON) WOTADNS_LED_OFF
        break;
      }
      else {
        Serial.print("*"); // print a '*' on the monitor
        // invert the LED state
        if(ledIsON) digitalWrite(ledPin, digitalRead(ledPin) ? LOW : HIGH);
        delay(500); // wait 500 mS
      }
    }
	if(ledIsON) WOTADNS_LED_OFF
		// if successful, flassh the LED rapidly and return true
	if(!wifiTimeout) {
		// if successful, flassh the LED rapidly 10 x 3 times
		for(uint8_t x = 0;x<10;x++) {
			if(ledIsON) WOTADNS_LED_ON
			delay(100);
			if(ledIsON) WOTADNS_LED_OFF
			delay(100);
		}
		ledOFF();	// return LED pin to INPUT
		return 0;
	}
    // unsuccessful connect, return false
	ledOFF();	// return LED pin to INPUT
	modeIsOTA = false;
	modeIsDNS = false;
	return -1;
}

void WOTADNS::beginOTA() {
  // OTA event handlers for debugging Serial printing at the remote device
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "Sketch";
    } else { // U_FS (SPIFFS)
      type = "Filesystem";
	  	// if updating SPIFFS,unmount SPIFFS using SPIFFS.end()
    	SPIFFS.end();
    }
    Serial.println("\r\nStart uploading \"" + type + "\"");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("End of OTA upload!");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    static uint8_t lastProg = 0;
    uint8_t prog = (progress / (total / 100));
    if (prog == 25 || prog == 50 || prog == 75 || prog == 100) {
      if (prog != lastProg) {
		  Serial.print("Progress: ");
		  Serial.print(prog);
		  Serial.println("%");
        lastProg = prog;
      }
    }
  });

  // OTA error printout
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  // start ArduinoOTA
  ArduinoOTA.begin();
}

// keep ArduinoOTA updated
void WOTADNS::refresh() {
	if(modeIsOTA) ArduinoOTA.handle();
	if(modeIsDNS) MDNS.update();
}

// print out useful details about the device/connection
void WOTADNS::printDetails() {
	uint8_t ns = 9;	// number of padding spaces in headings
	// print out the ESP8266 connection data
  Serial.println("\r\nWi-Fi Connected to...");
  Serial.println(pad("SSID",ns) + ": " + WiFi.SSID());
  Serial.print(pad("IP",ns) + ": ");
  Serial.println(WiFi.localIP());
  Serial.print(pad("SUBNET",ns) + ": ");
  Serial.println(WiFi.subnetMask());
  Serial.print(pad("GATEWAY",ns) + ": ");
  Serial.println(WiFi.gatewayIP());
  Serial.print(pad("HOSTNAME",ns) + ": ");
  Serial.println(WiFi.hostname());
  if(modeIsOTA || modeIsDNS) {
	  Serial.print(pad("NET NAME",ns) + ": ");
	  Serial.print(WiFi.hostname());
	  Serial.println(".local");
  }
  Serial.print(pad("MAC",ns) + ": ");
  Serial.println(WiFi.macAddress());
  Serial.print(pad("RSSI",ns) + ": ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.print(pad("FLASH",ns) + ": ");
  Serial.print(ESP.getFlashChipRealSize()/1000L);
  Serial.println(" KBytes");
  Serial.print(pad("CPU FREQ",ns) + ": ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");
    
}

void WOTADNS::ledON(uint8_t pin) {
	pinMode(pin,OUTPUT);
	ledPin = pin;
	ledIsON = true;
}

void WOTADNS::ledOFF(void) {
	if(ledIsON) pinMode(ledPin,INPUT);
	ledPin = -1;
	ledIsON = false;
}

String WOTADNS::pad(String str1, String str2, uint8_t len) {
	String pads;
	for(uint8_t x = 0;x<(len - str1.length());x++) pads += " ";
	return pads + str1 + str2;
}

String WOTADNS::pad(String str1, uint8_t len) {
	return pad(str1,"",len);
}