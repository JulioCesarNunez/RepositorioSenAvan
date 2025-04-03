#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Pines del LED RGB
#define PIN_R 18
#define PIN_G 19
#define PIN_B 23

// Variables para almacenar los ángulos
float angleX = 0, angleY = 0, angleZ = 0;
unsigned long prevTime = 0;  // Tiempo anterior para el cálculo de dt

void setup() {
    Serial.begin(57600);
    Wire.begin(21, 22); // SDA = GPIO 21, SCL = GPIO 22

    Serial.println("Iniciando MPU6050...");

    if (!mpu.begin()) {
        Serial.println("No se pudo encontrar el MPU6050. Verifica las conexiones.");
        while (1);
    }
    Serial.println("MPU6050 listo.");

    // Configuración del sensor
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    prevTime = millis(); // Inicializamos el tiempo

    // Configurar los pines del LED RGB como salida
    pinMode(PIN_R, OUTPUT);
    pinMode(PIN_G, OUTPUT);
    pinMode(PIN_B, OUTPUT);

    // Apagar el LED al inicio
    setColor(0, 0, 0);
}

void loop() {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    // Calcular el tiempo transcurrido (dt) en segundos
    unsigned long currentTime = millis();
    float dt = (currentTime - prevTime) / 1000.0; 
    prevTime = currentTime;

    // Integración para obtener los ángulos en grados
    angleX += gyro.gyro.x * dt * 57.2958; // Convertir rad/s a grados/s
    angleY += gyro.gyro.y * dt * 57.2958;
    angleZ += gyro.gyro.z * dt * 57.2958;

    // Normalizar los ángulos dentro del rango [-180°, 180°]
    angleX = fmod(angleX + 180, 360) - 180;
    angleY = fmod(angleY + 180, 360) - 180;
    angleZ = fmod(angleZ + 180, 360) - 180;

    // Determinar la posición y encender el color correspondiente
    String posicion = detectarPosicion(angleX, angleY, angleZ);

    // Imprimir los resultados en el Monitor Serie
    Serial.print("Ángulos (°): X=");
    Serial.print(angleX);
    Serial.print(" | Y=");
    Serial.print(angleY);
    Serial.print(" | Z=");
    Serial.print(angleZ);
    Serial.print(" | Posición: ");
    Serial.println(posicion);

    Serial.println("-------------------------");
    delay(500); // Pequeña pausa para estabilidad
}

// Función para detectar la posición del dado según los ángulos en X, Y y Z
String detectarPosicion(float x, float y, float z) {
    if (z > 50) {
        setColor(255, 0, 0);  // Rojo
        return "Cara 1 (Base)";
    }
    if (x > 50) {
        setColor(0, 255, 0);  // Verde
        return "Cara 2 (Lado derecho)";
    }
    if (x < -50) {
        setColor(0, 0, 255);  // Azul
        return "Cara 3 (Lado izquierdo)";
    }
    if (x < -50 && x > -70) { // Ampliar rango para evitar trabarse en izquierda
        setColor(0, 0, 255);  // Azul
        return "Cara 3 (Lado izquierdo - ajustado)";
    }
    if (y > 50) {
        setColor(255, 255, 0);  // Amarillo
        return "Cara 4 (Frontal)";
    }
    if (y < -50) {
        setColor(255, 0, 255);  // Magenta
        return "Cara 5 (Trasera)";
    }
    if (z < -50) {
        setColor(255, 165, 0);  // Naranja (Rojo + Verde en proporción)
        return "Cara 6 (Superior)";
    }

    // Si no está en ninguna cara exacta, apagar el LED
    setColor(0, 0, 0);
    return "Entre posiciones...";
}

// Función para controlar el color del LED RGB
void setColor(int r, int g, int b) {
    analogWrite(PIN_R, r);
    analogWrite(PIN_G, g);
    analogWrite(PIN_B, b);
}
