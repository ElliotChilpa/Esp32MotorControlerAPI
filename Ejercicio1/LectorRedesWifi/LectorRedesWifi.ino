#include "ESP8266WiFi.h"

void setup()
   {   Serial.begin(115200);
       WiFi.mode(WIFI_STA);
       WiFi.disconnect();
       delay(100);
       Serial.println("Configuracion completa");
   }                   

void loop()
{
  int n = WiFi.scanNetworks();
  if (n == 0)
  Serial.println("No encuentro redes disponibles");
  Serial.print(n);
  Serial.println(" networks found");


  for (int i = 0; i < n; ++i)
  {  // Print SSID y RSSI para cada una
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.print(")");
    Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
    delay(10);
  }
}