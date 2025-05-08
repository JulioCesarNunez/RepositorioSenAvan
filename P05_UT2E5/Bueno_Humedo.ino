#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

// Crear objetos de sensor
Adafruit_AHTX0 aht20;
Adafruit_BMP280 bmp280;

// Pin analógico para el sensor LM35
const int LM35_PIN = 34;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Inicializar AHT20
  if (!aht20.begin()) {
    Serial.println("Error: No se encontró el AHT20");
    while (1) delay(10);
  }

  // Inicializar BMP280
  if (!bmp280.begin(0x77)) {  // Cambia a 0x76 si tu módulo lo requiere
    Serial.println("Error: No se encontró el BMP280");
    while (1) delay(10);
  }

  bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,
                     Adafruit_BMP280::SAMPLING_X2,
                     Adafruit_BMP280::SAMPLING_X16,
                     Adafruit_BMP280::FILTER_X16,
                     Adafruit_BMP280::STANDBY_MS_500);

  // Imprimir encabezado CSV
  Serial.println("Temp_AHT20,Hum_AHT20,Temp_BMP280,Pressure_BMP280,Temp_LM35");
}

void loop() {
  // Lectura AHT20
  sensors_event_t humidity, tempAHT;
  aht20.getEvent(&humidity, &tempAHT);

  // Lectura BMP280
  float tempBMP = bmp280.readTemperature();
  float pressure = bmp280.readPressure() / 100.0;  // hPa

  // Lectura LM35
  int rawLM35 = analogRead(LM35_PIN);
  float voltageLM35 = (rawLM35 * 3.3) / 4095.0;  // conversión ADC a voltaje
  float tempLM35 = voltageLM35 * 100.0;          // 10 mV/°C

  // Imprimir en formato CSV
  
  Serial.print(humidity.relative_humidity, 2); Serial.print("% ,");
  Serial.print(tempAHT.temperature, 2); Serial.print("t1,");
  Serial.print(tempBMP, 2); Serial.print("t2,");
  Serial.print(tempLM35, 2); Serial.println("t3");

  delay(2000);
}
