#include <Wire.h>  // Biblioteca para la comunicación I2C
#include "Adafruit_VL53L0X.h"  // Biblioteca para el sensor de distancia VL53L0X

Adafruit_VL53L0X lox = Adafruit_VL53L0X();  // Crear un objeto para el sensor

const float alpha = 0.25;  // Factor de suavizado para el filtro Exponential Moving Average (EMA)
float filteredDistance = 0; // Valor inicial del filtro

void setup() {
    Serial.begin(115200);  // Iniciar la comunicación serie a 115200 baudios
    Wire.begin(21, 22);  // Configurar pines I2C en ESP32 (SDA = GPIO 21, SCL = GPIO 22)
    
    Serial.println("Iniciando VL53L0X...");
    
    // Verificar si el sensor se inicializa correctamente
    if (!lox.begin()) {
        Serial.println(F("¡Error al inicializar VL53L0X!"));
        while (1);  // Detener ejecución si hay error en la inicialización
    }
    
    Serial.println(F("VL53L0X listo para medir distancia."));
}

void loop() {
    VL53L0X_RangingMeasurementData_t measure;  // Estructura para almacenar datos de medición
    
    lox.rangingTest(&measure, false); // Obtener medición del sensor (false = sin debug extra)
    
    if (measure.RangeStatus != 4) {  // Verificar que la medición sea válida
        float rawDistance = measure.RangeMilliMeter; // Obtener la distancia sin filtrar

        // Aplicar filtro de promedio exponencial (EMA) para suavizar las mediciones
        filteredDistance = (alpha * rawDistance) + ((1 - alpha) * filteredDistance);

        // Imprimir en la consola serie los valores de distancia cruda y filtrada
        Serial.print("Distancia cruda: ");
        Serial.print(rawDistance);
        Serial.print(" mm | Distancia filtrada: ");
        Serial.print(filteredDistance);
        Serial.println(" mm");
    } else {
        Serial.println("Fuera de rango");  // Mensaje si la medición está fuera del rango del sensor
    }
    
    delay(100);  // Pequeño retraso para controlar la frecuencia de medición
}
