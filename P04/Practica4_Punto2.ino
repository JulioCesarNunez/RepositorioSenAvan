/*
  =============================================
  Fenología del Frijol con sensor BMP180
  ESP32 + I2C (SDA 21, SCL 22)
  =============================================
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Constantes
const float TB = 10.0; // Umbral térmico (°C)
float UC_acumuladas = 0.0;

// Simulación de mín y máx
float Tmax = -99999;
float Tmin = 99999;
unsigned long ultimoMomento = 0;
const unsigned long intervaloDia = 10 * 1000; // 10s = 1 día simulado

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22); // Pines I2C ESP32

  Serial.println("Iniciando sensor BMP180...");

  if (!bmp.begin()) {
    Serial.println("No se detectó el sensor BMP180. Revisa conexiones.");
    while (true);
  }

  Serial.println(" Sensor BMP180 inicializado.\n");
  delay(2000);
}

void loop() {
  float tempC;
  bmp.getTemperature(&tempC); // Obtener temperatura en °C

  // Simular registro de Tmax y Tmin durante un día
  if (tempC > Tmax) Tmax = tempC;
  if (tempC < Tmin) Tmin = tempC;

  // Simular un día cada 10 segundos
  if (millis() - ultimoMomento >= intervaloDia) {
    float UC_diaria = ((Tmax + Tmin) / 2.0) - TB;
    if (UC_diaria < 0) UC_diaria = 0;

    UC_acumuladas += UC_diaria;

    Serial.println(" Día simulado:");
    Serial.print("Tmax: "); Serial.print(Tmax, 2); Serial.print(" °C | ");
    Serial.print("Tmin: "); Serial.print(Tmin, 2); Serial.println(" °C");

    Serial.print("UC diaria: "); Serial.println(UC_diaria, 2);
    Serial.print("UC acumuladas: "); Serial.println(UC_acumuladas, 2);
    Serial.print("Etapa fenológica: ");
    Serial.println(determinarEtapa(UC_acumuladas));
    Serial.println("--------------------------------------");

    // Reiniciar valores para siguiente día
    Tmax = -99999;
    Tmin = 99999;
    ultimoMomento = millis();
  }

  delay(1000); // Medición cada segundo
}

String determinarEtapa(float UC) {
  if (UC < 12.89) return "Siembra (pre-emergencia)";
  else if (UC < 64.41) return "Emergencia";
  else if (UC < 91.33) return "Formación de guías";
  else if (UC < 99.31) return "Floración";
  else if (UC < 112.77) return "Formación de vaina";
  else if (UC < 153.24) return "Llenado de vainas";
  else return "Maduración";
}
