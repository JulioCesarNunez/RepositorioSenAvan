#include <Wire.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const float alpha = 0.25;  // Factor de suavizado del filtro EMA
float filteredDistance = 0; // Valor filtrado inicializado

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);  // Configurar I2C en ESP32 (SDA = GPIO 21, SCL = GPIO 22)
    
    Serial.println("Iniciando VL53L0X...");
    
    if (!lox.begin()) {
        Serial.println(F("¡Error al inicializar VL53L0X!"));
        while (1);
    }
    
    Serial.println(F("VL53L0X listo para medir distancia."));
}

void loop() {
    VL53L0X_RangingMeasurementData_t measure;
    
    lox.rangingTest(&measure, false); // false = sin debug extra
    
    if (measure.RangeStatus != 4) {  // Si no hay error en la medición
        float rawDistance = measure.RangeMilliMeter; // Distancia sin filtrar

        // Aplicar filtro EMA
        filteredDistance = (alpha * rawDistance) + ((1 - alpha) * filteredDistance);

        Serial.print("Distancia cruda: ");
        Serial.print(rawDistance);
        Serial.print(" mm | Distancia filtrada: ");
        Serial.print(filteredDistance);
        Serial.println(" mm");
    } else {
        Serial.println("Fuera de rango");
    }
    
    delay(100);
}
