#include <WOTADNS.h>

void setup() {
  DISABLE_WIFI
  SERIAL_BEGIN(115200)

  Serial.println("\r\n\r\nHello World!");

  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  delay(1000);
}
