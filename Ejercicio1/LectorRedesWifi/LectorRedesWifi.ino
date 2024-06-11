// Definición de pines
const int potPin = A0; // Pin analógico A0
const int ledPin = 2; // Pin digital D1 (GPIO5) para el LED

void setup() {
  // Inicializar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  // Inicializar la comunicación serial para monitorear valores
  Serial.begin(115200);
}

void loop() {
  // Leer el valor del potenciómetro
  int potValue = analogRead(potPin);
  
  // Mapear el valor leído (0-1023) a un rango adecuado para PWM (0-255)
  int brightness = map(potValue, 0, 1023, 0, 255);
  
  // Ajustar el brillo del LED usando PWM
  analogWrite(ledPin, brightness);
  
  // Imprimir el valor leído en el monitor serial
  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print(" - LED Brightness: ");
  Serial.println(brightness);
  
  // Esperar un pequeño intervalo antes de la siguiente lectura
  delay(100);
}
