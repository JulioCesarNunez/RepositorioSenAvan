#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

// Crear objeto del sensor BMP085
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Humedad relativa constante (49.22%)
const float HR = 0.4922;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
// Inicializar I2C en ESP32
  Wire.begin(21, 22);

  Serial.println("Iniciando sensor BMP180...");

  if (!bmp.begin()) {
    Serial.println(" Error al detectar el sensor BMP085");
    while (1);
  }

  Serial.println(" Sensor BMP085 inicializado correctamente");
  Serial.println("==============================================\n");
}

void loop() {
  sensors_event_t event;
  bmp.getEvent(&event);

  if (event.pressure) {
    // Obtener temperatura
    float tempC;
    bmp.getTemperature(&tempC);

    // Obtener presión en hPa
    float pressure_hPa = event.pressure;

    // Aplicar la fórmula:
    // ρa = [0.348444 * p - hr(0.00252 * t - 0.020582)] / (273.15 + t)
    float numerador = 0.348444 * pressure_hPa - HR * (0.00252 * tempC - 0.020582);
    float denominador = 273.15 + tempC;
    float densidad_aire = numerador / denominador;

    // Mostrar resultados
    Serial.print("Temperatura: ");
    Serial.print(tempC, 2);
    Serial.println(" °C");

    Serial.print("Presión: ");
    Serial.print(pressure_hPa, 2);
    Serial.println(" hPa");

    Serial.print("Densidad del aire (kg/m³): ");
    Serial.println(densidad_aire, 6);

    Serial.println("-------------------------------------------------\n");
  } else {
    Serial.println("Error al leer presión.");
  }

  delay(3000);
}
