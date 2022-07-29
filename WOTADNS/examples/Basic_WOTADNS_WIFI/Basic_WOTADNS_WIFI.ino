/*
  Updated ArduinoOTA with helper library WOTADNS
  (C) 2022 Reverend Phil Morris
*/

#include <WOTADNS.h> // helper functions for ArduinoOTA and WiFi connect
#include <WiFiCredentials.h>  // Wi-Fi credentials header file

WOTADNS wifi;

#ifndef STA_SSID
// Replace with your network credentials
const char* STA_SSID = "<ssid>";
const char* STA_PASS = "<password>";
#endif


void setup() {
  WiFi.disconnect();  // disconnect any previous connection
  WiFi.persistent(false); // do not save credentials to Flash
  SERIAL_BEGIN(115200);
  while(!Serial);
  Serial.println("\r\n\r\n\r\nStarting Wi-Fi...");
  // configure Wi-Fi for DHCP
  WiFi.mode(WIFI_STA);
  // flash the built-in LED during connection
  wifi.ledON(LED_BUILTIN);  // or, choose a pin of your choice
  // SSID, PASSWORD, HOSTNAME (optional for Wi-Fi only)
  if(wifi.beginWiFi(STA_SSID, STA_PASS) < 0) {
    Serial.println("Wi-Fi connect failed!");
    delay(2000);
    ESP.restart();
  }

  // print out the Wi-Fi connection details
  wifi.printDetails();

}//END OF setup()


void loop() {
  wifi.refresh();
  
  //<your code here>
  
}//END OF loop()
