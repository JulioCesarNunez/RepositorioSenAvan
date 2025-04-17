#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

// Crear el objeto del sensor BMP085
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Humedad relativa (valor fijo en fracción)
const float HR = 0.4922;

void setup() {
  Serial.begin(19200);
  delay(1000);

  Wire.begin(21, 22); // Pines I2C ESP32

  Serial.println("Iniciando sensor MPL3115A2...");

  if (!bmp.begin()) {
    Serial.println(" No se pudo encontrar el sensor BMP085");
    while (1);
  }

  Serial.println("Sensor BMP085 inicializado correctamente");
  Serial.println("==========================================\n");
}

void loop() {
  sensors_event_t event;
  bmp.getEvent(&event);

  if (event.pressure) {
    // Obtener temperatura
    float tempC;
    bmp.getTemperature(&tempC);

    // Presión en hPa
    float pressure_hPa = event.pressure;

    // Cálculo de densidad del aire usando la fórmula dada
    float numerador = 0.348444 * pressure_hPa - HR * (0.00252 * tempC - 0.020582);
    float denominador = 273.15 + tempC;
    float densidad_aire = numerador / denominador;

    // Altitud estimada en metros usando presión
    float altitud = bmp.pressureToAltitude(1013.25, pressure_hPa); // 1013.25 = presión al nivel del mar

    // Imprimir resultados
    Serial.print("  Temperatura: ");
    Serial.print(tempC, 2);
    Serial.println(" °C");

    Serial.print("Presión: ");
    Serial.print(pressure_hPa, 2);
    Serial.println(" hPa");

    Serial.print("Altitud estimada: ");
    Serial.print(altitud, 2);
    Serial.println(" m");

    Serial.print(" Densidad del aire: ");
    Serial.print(densidad_aire, 6);
    Serial.println(" kg/m³");

    Serial.println("---------------------------------------------------\n");
  } else {
    Serial.println("Error al leer presión del sensor.");
  }

  delay(5000); // Esperar 5 segundos
}