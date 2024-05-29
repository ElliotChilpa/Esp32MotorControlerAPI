/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/MotorDer will set the GPIO2 low,
 *    http://server_ip/MotorIzq will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

 /*
 *  Nos Puede servir el GPIO 1 que es TX en la plac
 *  Nos Puede servir el GPIO 3 que es RX
 *  Nos Puede servir el GPIO 15 que es D8 en la placa
 *  Nos Puede servir el GPIO 13 que es el D7
 *  Nos Puede servir el GPIO 12 que es el D6
 *  Nos Puede servir el GPIO 14 que es el D5
 */

#include <ESP8266WiFi.h>

const char* ssid = "Totalplay-C4A5";
const char* password = "C4A5E9782q3g346Q";



// Creamos una instancia para el servidor
// con server especificamoes el puerto 
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1500);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  pinMode(1, OUTPUT);
  digitalWrite(1, LOW);

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  int derM1;
  int izqM1;
  if (req.indexOf("/der") != -1)
  {
    val = 0;
    derM1 = 0;
    izqM1 = 1;
  }
  else if (req.indexOf("/izq") != -1)
  {
    val = 1;
    derM1 = 1;
    izqM1 = 0;
  }
  /*else if (req.indexOf("/Para") != -1)
  }
    val = 0;
    derM1 = 0;
    izqM1 = 0;
  }*/
  else 
  {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(2, val);
  
  // Pines puestos por Elliot Chilpa
  digitalWrite(1, derM1);
  digitalWrite(3, izqM1);

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

