#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;

void setup() {
    Serial.begin(9600);
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

    // Cálculo de ángulos de inclinación
    float roll  = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / M_PI;
    float pitch = atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + accel.acceleration.z * accel.acceleration.z)) * 180.0 / M_PI;

    // Determinar la posición
    String position = "Base";
    if (pitch > 45) {
        position = "Frente";
    } else if (roll > 45) {
        position = "Derecha";
    } else if (roll < -45) {
        position = "Izquierda";
    }

    // Imprimir la posición
    Serial.println(position);
    
    delay(500);
}

