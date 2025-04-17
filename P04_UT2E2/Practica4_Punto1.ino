/*
  ========================================================
  Sensor BMP180 con ESP32 - Densidad del aire y
  presión de vapor de saturación (Buck)
  ========================================================

  CONEXIONES:
  --------------------------
  BMP180       →   ESP32
  --------------------------
  VCC          →   3.3V
  GND          →   GND
  SDA          →   GPIO21
  SCL          →   GPIO22

  Librerías necesarias:
  - Adafruit BMP085 Unified (compatible con BMP180)
  - Adafruit Sensor
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Constante del gas ideal para aire seco (J/kg·K)
const float R = 287.05;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inicializar I2C en ESP32
  Wire.begin(21, 22);

  Serial.println("Iniciando sensor BMP180...");

  if (!bmp.begin()) {
    Serial.println("No se encontró el sensor BMP180. Verifica conexiones.");
    while (true);
  }

  Serial.println("Sensor BMP180 detectado.");
  Serial.println("=====================================\n");
}

void loop() {
  sensors_event_t pressure_event;
  bmp.getEvent(&pressure_event);

  if (pressure_event.pressure) {
    float tempC;
    bmp.getTemperature(&tempC);

    float tempK = tempC + 273.15;
    float presion_Pa = pressure_event.pressure * 100.0; // hPa a Pa

    // Densidad del aire (kg/m³)
    float densidad = presion_Pa / (R * tempK);

    // Presión de vapor de saturación (kPa) con fórmula de Buck
    float p_sv_kPa = calcular_psv(tempC);
    float p_sv_Pa = p_sv_kPa * 1000.0;

    // Mostrar datos
    Serial.println(" Medición actual:");
    Serial.print("Temperatura: ");
    Serial.print(tempC, 2);
    Serial.println(" °C");

    Serial.print("Presión: ");
    Serial.print(presion_Pa, 2);
    Serial.println(" Pa");

    Serial.print("Densidad del aire: ");
    Serial.print(densidad, 4);
    Serial.println(" kg/m³");

    Serial.print("Presión de vapor de saturación: ");
    Serial.print(p_sv_kPa, 3);
    Serial.println(" kPa");

    Serial.println("------------------------------\n");
  } else {
    Serial.println(" No se pudo leer la presión.");
  }

  delay(3000);
}

// Fórmula de Buck para presión de vapor de saturación (en kPa)
float calcular_psv(float T) {
  return 0.61121 * exp((17.502 * T) / (T + 240.97));
}

