//Codigo con filtro 

const int pinAnalogMQ2 = 34; // AOUT
float R0 = 10.0; // Valor estimado de R0 (se puede calibrar automáticamente)

// Parámetros de la curva del MQ-2 para humo (del datasheet)
// log(ppm) = (log(Rs/R0) - b) / m
float m = -0.42;  // pendiente de la curva log-log para humo
float b = 1.92;   // intersección

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Calibración inicial (en aire limpio)
  Serial.println("Calibrando R0 en aire limpio...");
  float suma = 0;
  for (int i = 0; i < 100; i++) {
    suma += calcularRs();
    delay(100);
  }
  R0 = suma / 100.0;
  Serial.print("R0 calibrado: ");
  Serial.println(R0);
}

void loop() {
  float Rs = calcularRs();
  float ratio = Rs / R0;

  float ppm = pow(10, (log10(ratio) - b) / m);

  Serial.print("Rs: ");
  Serial.print(Rs);
  Serial.print(" | Rs/R0: ");
  Serial.print(ratio);
  Serial.print(" | Estimado ppm (humo): ");
  Serial.println(ppm);

  delay(2000);
}

float calcularRs() {
  int adc = analogRead(pinAnalogMQ2);
  float voltage = adc * 3.3 / 4095.0; // conversión para ESP32 (resolución de 12 bits)
  float RL = 10.0; // Resistencia de carga en kΩ (verifica la del módulo que estás usando)

  // Usando la fórmula: Rs = RL * (Vcc - Vout) / Vout
  float Rs = RL * (3.3 - voltage) / voltage;
  return Rs;
}

