#include "ESP8266WiFi.h"
#include <ArduinoJson.h>

const char* wifiName = "INFINITUM7B82"; //const char* wifiName = "Wi-Fi IPN";
const char* wifiPass = "ggYfLbvgk5";
int contconexion = 0;

WiFiServer server(80);

String header;
String requestBody;
const int salida = 2;
const int salida2 = 13;
const int salida3 = 15;

void setup() {
    pinMode(salida, OUTPUT);
    pinMode(salida2, OUTPUT); 
    pinMode(salida3, OUTPUT); 

    Serial.begin(115200);
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifiName);
    WiFi.begin(wifiName, wifiPass);
    while (WiFi.status() != WL_CONNECTED && contconexion < 50) {
        ++contconexion;
        delay(500);
        Serial.print(".");
    }
    if (contconexion < 50) {
        Serial.println("");
        Serial.println("WiFi conectado");
        Serial.println(WiFi.localIP());
        server.begin();
    } else {
        Serial.println("");
        Serial.println("Error de conexion");
    }
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("New Client.");
        header = "";
        requestBody = "";
        bool isPost = false;
        int contentLength = 0;

        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                header += c;  
                Serial.write(c); // Imprimir en Serial Monitor

                if (header.endsWith("\r\n\r\n")) {
                    if (header.startsWith("POST")) {
                        isPost = true;
                        int contentIndex = header.indexOf("Content-Length: ");
                        if (contentIndex >= 0) {
                            contentLength = header.substring(contentIndex + 16).toInt();
                        }
                    }
                    break;
                }
            }
        }
        if (isPost && contentLength > 0) {
            while (client.available() && requestBody.length() < contentLength) {
                char c = client.read();
                requestBody += c;
            }
            Serial.print("Request Body: ");
            Serial.println(requestBody);
        }
        
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type: application/json");
        client.println("Connection: close");
        client.println();
        if (header.indexOf("POST /estado") >= 0) {
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, requestBody);
            if (error) {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
                client.println("{\"status\":\"1 - Error\"}");
            } else {
                int estado = doc["estado"];
                Serial.print("Estado recibido: ");
                Serial.println(estado);
                if (estado == 1) {
                    digitalWrite(salida, LOW); //Encender
                    digitalWrite(15, LOW); //HIGH ES 0 Y LOW ES 255
                    digitalWrite(13, HIGH); //HIGH ES 0 Y LOW ES 255
                    client.println("{\"status\":\"0 - Ok\",\"Modo\":\"Encendido\"}");
                } else if (estado == 0) {
                    digitalWrite(salida, HIGH); //Apagar
                    digitalWrite(15, LOW); //HIGH ES 0 Y LOW ES 255
                    digitalWrite(13, LOW); //HIGH ES 0 Y LOW ES 255
                    client.println("{\"status\":\"0 - Ok\",\"Modo\":\"Apagado\"}");
                }
            }
        } else if (header.indexOf("POST /direccion") >= 0) {
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, requestBody);
            if (error) {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
                client.println("{\"status\":\"1 - Error\"}");
            } else {
                int direccion = doc["direccion"];
                Serial.print("Recibo la direccion del motor: ");
                Serial.println(direccion);

                if (direccion == 1) { //Atras
                    digitalWrite(salida, HIGH);
                    digitalWrite(15, HIGH); //HIGH ES 0 Y LOW ES 255
                    digitalWrite(13, LOW); //HIGH ES 0 Y LOW ES 255
                    client.println("{\"status\":\"0 - Ok\",\"Modo\":\"Atras\"}");
                } else if (direccion == 0) { //Adelante
                    digitalWrite(salida, HIGH);
                    digitalWrite(15, LOW); //HIGH ES 0 Y LOW ES 255
                    digitalWrite(13, HIGH); //HIGH ES 0 Y LOW ES 255
                    client.println("{\"status\":\"0 - Ok\",\"Modo\":\"Adelante\"}");
                } else if (direccion == 2) { //Estacionado
                digitalWrite(salida, HIGH); 
                digitalWrite(15, LOW); //HIGH ES 0 Y LOW ES 255
                digitalWrite(13, LOW); //HIGH ES 0 Y LOW ES 255
                client.println("{\"status\":\"0 - Ok\",\"Modo\":\"Estacionado\"}");
                }
            }
        } else {
            client.println("{\"status\":\"1 - Error\"}");
        }

        client.println();
        client.stop();
        Serial.println("Client disconnected.");
    }
}
