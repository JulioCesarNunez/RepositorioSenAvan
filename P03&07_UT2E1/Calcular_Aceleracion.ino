#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;
const float masa = 0.5; // Masa del objeto en kg (ajustable según necesidad)

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22); // SDA en GPIO 21, SCL en GPIO 22

    if (!mpu.begin()) {
        Serial.println("No se pudo encontrar el MPU6050. Verifica las conexiones.");
        while (1);
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    delay(50);
}

void loop() {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    // Obtener la aceleración en el eje Y
    float aceleracion = accel.acceleration.y; // m/s^2

    // Calcular la fuerza aplicada (F = m * a)
    float fuerza = masa * aceleracion;

    // Simular la distancia de empuje entre 3 y 10 cm
    float distancia = random(3, 11) / 100.0; // Convertir cm a metros

    // Imprimir resultados
    Serial.print("Fuerza: ");
    Serial.print(fuerza);
    Serial.println(" N");
    
    
    delay(1000);
}


