#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>

const char* wifiName     = "Wi-Fi IPN";
const char* wifiPass = "";
int contconexion = 0;
//------------------Servidor Web en puerto 80---------------------
WiFiServer server(80);

//---------------------VARIABLES GLOBALES-------------------------

String header; // Variable para guardar el HTTP request++++++++++
String estadoSalida = "off";
const int salida = 2;
const int salidaServo = 15;
const int salidaServo2 = 13;
char mensaje[50];
//---------------------------SETUP--------------------------------


void setup() {  
    pinMode(salida, OUTPUT); 
        pinMode(salidaServo, OUTPUT); 
            pinMode(salidaServo2, OUTPUT); 
    Serial.begin(115200);
    delay(10);
  // We start by connecting to a WiFi network
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(wifiName);

  WiFi.begin(wifiName, wifiPass);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
      ++contconexion;
      delay(500);
      Serial.print(".");
    }
    if (contconexion <50) {
        Serial.println("");
        Serial.println("WiFi conectado");
        Serial.println(WiFi.localIP());
        server.begin(); // iniciamos el servidor
    }
    else { 
        Serial.println("");
        Serial.println("Error de conexion");
    }
}

//----------------------------LOOP----------------------------------

void loop(){
  WiFiClient client = server.available();   // Escucha a los clientes entrantes
  if (client) {                             // Si se conecta un nuevo cliente
    Serial.println("New Client.");          // 
    String currentLine = "";
    
    while (client.connected()) {            // loop mientras el cliente está conectado
      if (client.available()) {             // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
          
            // enciende y apaga el GPIO
            if (header.indexOf("GET /encender") >= 0) {
              Serial.println("GPIO encendido");
                digitalWrite(2, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(15, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(13, HIGH); //HIGH ES 0 Y LOW ES 255


            } else if (header.indexOf("GET /apagar") >= 0) {
              Serial.println("GPIO apagado");
                digitalWrite(2, HIGH); //HIGH ES 0 Y LOW E  S 255
                digitalWrite(15, HIGH); //HIGH ES 0 Y LOW ES 255
                digitalWrite(13, LOW); //HIGH ES 0 Y LOW ES 255
            }else if (header.indexOf("GET /detener") >= 0) {
              Serial.println("GPIO apagado");
                digitalWrite(2, HIGH); //HIGH ES 0 Y LOW E  S 255
                digitalWrite(15, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(13, LOW); //HIGH ES 0 Y LOW ES 255
            }

            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}