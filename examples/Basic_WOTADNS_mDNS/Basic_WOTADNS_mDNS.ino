/*
  Updated Wi-fi, ArduinoOTA/mDNS with helper library WOTADNS
  (C) 2022 Reverend Phil Morris
*/

#include <WOTADNS.h> // helper functions for ArduinoOTA and WiFi connect
#include <WiFiCredentials.h>  // Wi-Fi credentials header file

#define HOSTNAME "mymdnshost"

WOTADNS mdns;

#ifndef STA_SSID
// Replace with your network credentials
const char* STA_SSID = "<ssid>";
const char* STA_PASS = "<password>";
#endif


void setup() {
  WiFi.disconnect();  // disconnect any previous connection
  WiFi.persistent(false); // do not save credentials to Flash
  SERIAL_BEGIN(115200);
  Serial.println("Starting Wi-Fi...");
  // configure Wi-Fi for DHCP
  WiFi.mode(WIFI_STA);

  // configure ArduinoOTA (optional)
  // mdns.setOtaPort(8266);  // Port defaults to 8266
  // mdns.setOtaPassword((const char *)"123"); // No authentication by default

  mdns.ledON(LED_BUILTIN);  // flash the onboard LED during Wi-Fi connection

  // SSID, PASSWORD and HOSTNAME for OTA
  if(mdns.beginWiFiDNS(STA_SSID, STA_PASS,HOSTNAME) < 0) {
    Serial.println("Wi-Fi connect failed!");
    delay(2000);
    ESP.restart();
  }

  // print out the Wi-Fi connection details
  mdns.printDetails();

}//END OF setup()


void loop() {
  mdns.refresh();
  
  //<your code here>
  
}//END OF loop()
