#include <ESP8266WiFi.h>

const char* ssid = "charly";     // Aqui van vuestros datos
const char* password = "contrase";

void setup() {
  // put your setup code here, to run once:
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {   
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
