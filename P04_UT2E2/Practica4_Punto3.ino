#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#define LM35_PIN 34

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

unsigned long startTime;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // Pines I2C para ESP32 (SDA = 21, SCL = 22)
  analogReadResolution(12); // Resolución ADC del ESP32

  // Iniciar sensor BMP085/BMP180
  if (!bmp.begin()) {
    Serial.println("❌ Error al iniciar BMP085/BMP180");
    while (1);
  }

  startTime = millis();

  // Encabezado CSV
  Serial.println("Tiempo (s),LM35 (°C),BMP180 (°C)");
}

void loop() {
  // Tiempo desde el inicio (en segundos)
  float tiempo = (millis() - startTime) / 1000.0;

  // Leer LM35
  int raw = analogRead(LM35_PIN);
  float voltaje = raw * (3.3 / 4095.0); // Conversión ADC (12 bits)
  float tempLM35 = voltaje * 100.0;     // LM35 entrega 10mV/°C

  // Leer BMP180
  sensors_event_t event;
  bmp.getEvent(&event); // Aunque no se usa la presión aquí, se requiere para el flujo de lectura
  float tempBMP = 0.0;
  bmp.getTemperature(&tempBMP);

  // Imprimir datos en formato CSV
  Serial.print(tiempo, 2); Serial.print(",");
  Serial.print(tempLM35, 2); Serial.print(",");
  Serial.println(tempBMP, 2);

  delay(1000); // Esperar 1 segundo
}
