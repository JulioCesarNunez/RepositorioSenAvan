#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Parámetros de detección
const float UMBRAL_DETECCION = 50.0; // Pa
const float DISTANCIA_MANGUERA = 0.3; // metros

// Variables
float presionBase = 0.0;
bool vehiculoDetectado = false;
unsigned long tiempoInicioVehiculo = 0;
unsigned long tiempoFinVehiculo = 0;
float deltaPresionMax = 0;
int contadorVehiculos = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // Pines I2C para ESP32
  if (!bmp.begin()) {
    Serial.println("Error al iniciar sensor BMP180");
    while (1);
  }

  // Calibrar presión base
  delay(2000);
  sensors_event_t event;
  bmp.getEvent(&event);
  presionBase = event.pressure * 100; // hPa a Pa

  Serial.println("Tiempo(s),Presión(Pa),ΔP(Pa),Velocidad(km/h),Tipo de vehículo,Total vehículos");
}

void loop() {
  sensors_event_t event;
  bmp.getEvent(&event);
  float presionActual = event.pressure * 100; // hPa a Pa
  float deltaP = presionActual - presionBase;
  unsigned long tiempoActual = millis();

  if (!vehiculoDetectado && deltaP >= UMBRAL_DETECCION) {
    // Comienza paso del vehículo
    vehiculoDetectado = true;
    tiempoInicioVehiculo = tiempoActual;
    deltaPresionMax = deltaP;
  }

  if (vehiculoDetectado) {
    // Actualiza el deltaP máximo
    if (deltaP > deltaPresionMax) deltaPresionMax = deltaP;

    // Fin del paso del vehículo
    if (deltaP < (UMBRAL_DETECCION / 2)) {
      vehiculoDetectado = false;
      tiempoFinVehiculo = tiempoActual;
      contadorVehiculos++;

      // Calcular duración y velocidad
      float duracionSeg = (tiempoFinVehiculo - tiempoInicioVehiculo) / 1000.0;
      float velocidad = (DISTANCIA_MANGUERA / duracionSeg) * 3.6; // m/s a km/h

      // Clasificación del vehículo
      String tipo;
      if (deltaPresionMax < 80 && duracionSeg < 0.5)
        tipo = "Bicicleta";
      else if (deltaPresionMax < 200)
        tipo = "Ligero";
      else
        tipo = "Pesado";

      // Imprimir resultados
      Serial.print(tiempoActual / 1000.0, 2); Serial.print(",");
      Serial.print(presionActual, 2); Serial.print(",");
      Serial.print(deltaPresionMax, 2); Serial.print(",");
      Serial.print(velocidad, 2); Serial.print(",");
      Serial.print(tipo); Serial.print(",");
      Serial.println(contadorVehiculos);
    }
  }

  delay(100);
}

