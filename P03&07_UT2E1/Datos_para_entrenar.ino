#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;

int mapValue(float x, float in_min, float in_max, int out_min, int out_max) {
    return (int)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

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
    
    // Mapeo de ángulos a rango 0-1023
    char rollMapped = mapValue(roll, -180, 180, 0, 1023);
    char pitchMapped = mapValue(pitch, -180, 180, 0, 1023);

    // Imprimir en formato <PPPP,RRRR>
    Serial.print('<');
    Serial.print(pitchMapped/1000);
    Serial.print((pitchMapped/100) % 10);
    Serial.print((pitchMapped/10) % 10);
    Serial.print(pitchMapped % 10);

    Serial.print(',');

    Serial.print(rollMapped / 1000);
    Serial.print((rollMapped / 100) % 10);
    Serial.print((rollMapped / 10) % 10);
    Serial.print(rollMapped % 10);
    
    Serial.print('>');

    delay(100);
}
