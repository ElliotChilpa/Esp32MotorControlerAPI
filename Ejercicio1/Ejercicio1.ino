#include <ESP8266WiFi.h>

const char* ssid = "Wi-Fi IPN";
const char* password = "";

WiFiServer server(80);


void setup()
   {  Serial.begin(115200);
      delay(1500);

      pinMode(2, OUTPUT);      // GPIO2
      digitalWrite(2, LOW);

      Serial.print("Connecting to ");
      Serial.println(ssid);
      WiFi.begin(ssid, password);

      while (WiFi.status() != WL_CONNECTED)
         {   delay(500);
             Serial.print(".");
         }
      Serial.println("WiFi connected");

      server.begin();                                // Iniciamos el servidor
      Serial.println("Server started");
      Serial.println(WiFi.localIP());      // Imprimimos la IP
   }

   WiFiClient client = server.available();
if (!client)
     return;

Serial.println("new client");
while(!client.available())
      delay(1);



      String req = client.readStringUntil('\r');
Serial.println(req);
client.flush();



int val;
if ( req.indexOf("/gpio/0") != -1)
     val = 0;
else if (req.indexOf("/gpio/1") != -1)
     val = 1;
else
   { Serial.println("invalid request");
     client.stop();
     return;
}

digitalWrite(2, val);



String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
s += (val)?"high":"low";
s += "</html>\n";

client.print(s);                    // Enviar el resultado de val al cliente
delay(1);
Serial.println("Client disonnected");