#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>

const char* wifiName     = "Wi-Fi IPN";
const char* wifiPass = "";


// const char* wifiName = "Totalplay-C4A5";
// const char* wifiPass = "C4A5E9782q3g346Q";

int contconexion = 0;
//------------------Servidor Web en puerto 80---------------------
WiFiServer server(80);

//---------------------VARIABLES GLOBALES-------------------------

String header; // Variable para guardar el HTTP request++++++++++
String estadoSalida = "off";
const int salida = 2;
const int salidaServo = 15;
const int salidaServo2 = 13;
const int salidaServo3 = 14;
const int salidaServo4 = 12;

int PWMA = 0; // Pin PWM para velocidad
int PWMB = 4;

//int potPin = A0; // Pin analógico al que se conecta el potenciómetro


char mensaje[50];
//---------------------------SETUP--------------------------------


void setup() {  
    pinMode(salida, OUTPUT); 
    pinMode(salidaServo, OUTPUT); 
    pinMode(salidaServo2, OUTPUT);
    pinMode(salidaServo3, OUTPUT); 
    pinMode(salidaServo4, OUTPUT); 

    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);
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
            if (header.indexOf("GET /avanzar") >= 0) {
              Serial.println("GPIO avanzar");
    
                // Leer el valor del potenciómetro
                //int potValue = analogRead(potPin);

                // Mapear el valor leído (0-1023) a un rango adecuado para PWM (0-255)
                //int velocidad = map(potValue, 0, 1023, 0, 255);

                analogWrite(PWMA, 70); // Máxima velocidad
                analogWrite(PWMB, 70); // Máxima velocidad
                digitalWrite(2, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(15, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(13, HIGH); //HIGH ES 0 Y LOW ES 255
                digitalWrite(14, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(12, HIGH); //HIGH ES 0 Y LOW ES 255



            } 
            else if (header.indexOf("GET /x") >= 0) 
            {
              Serial.println("GPIO reversa");

                // Leer el valor del potenciómetro
                //int potValue = analogRead(potPin);

                // Mapear el valor leído (0-1023) a un rango adecuado para PWM (0-255)
                // int velocidad = map(potValue, 0, 1023, 0, 255);

                analogWrite(PWMA, 70); // Máxima velocidad
                analogWrite(PWMB, 70); // Máxima velocidad
                digitalWrite(2, HIGH); //HIGH ES 0 Y LOW E  S 255
                digitalWrite(15, HIGH); //HIGH ES 0 Y LOW ES 255
                digitalWrite(13, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(14, HIGH); //HIGH ES 0 Y LOW ES 255
                digitalWrite(12, LOW); //HIGH ES 0 Y LOW ES 255
            }
            else if (header.indexOf("GET /derecha") >= 0) 
            {
              Serial.println("GPIO derecha");

                // Leer el valor del potenciómetro
                // int potValue = analogRead(potPin);

                // Mapear el valor leído (0-1023) a un rango adecuado para PWM (0-255)
                // int velocidad = map(potValue, 0, 1023, 0, 255);

                analogWrite(PWMA, 70); // Máxima velocidad
                analogWrite(PWMB, 70); // Máxima velocidad
                // LED
                digitalWrite(2, HIGH); //HIGH ES 0 Y LOW E  S 255
                // MOTOR 1
                digitalWrite(15, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(13, HIGH); //HIGH ES 0 Y LOW ES 255
                // MOTOR 2
                digitalWrite(14, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(12, LOW); //HIGH ES 0 Y LOW ES 255
            }
            else if (header.indexOf("GET /izquierda") >= 0) 
            {
              Serial.println("GPIO izquierda");

                // Leer el valor del potenciómetro
                // int potValue = analogRead(potPin);

                // Mapear el valor leído (0-1023) a un rango adecuado para PWM (0-255)
                // int velocidad = map(potValue, 0, 1023, 0, 255);

                analogWrite(PWMA, 70); // Máxima velocidad
                analogWrite(PWMB, 70); // Máxima velocidad
                // LED
                digitalWrite(2, HIGH); //HIGH ES 0 Y LOW E  S 255
                // MOTOR 1
                digitalWrite(15, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(13, LOW); //HIGH ES 0 Y LOW ES 255
                // MOTOR 2
                digitalWrite(14, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(12, HIGH); //HIGH ES 0 Y LOW ES 255
            }
            else if (header.indexOf("GET /detener") >= 0) 
            {
              Serial.println("GPIO apagado");
                analogWrite(PWMA, 0); // Máxima velocidad
                analogWrite(PWMB, 0); // Máxima velocidad
                digitalWrite(2, HIGH); //HIGH ES 0 Y LOW E  S 255
                digitalWrite(15, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(13, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(14, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(12, LOW); //HIGH ES 0 Y LOW ES 255
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