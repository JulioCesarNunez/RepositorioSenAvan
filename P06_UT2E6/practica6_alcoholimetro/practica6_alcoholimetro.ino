//alcoholimetro
const int pinAnalogMQ2 = 34; // AOUT
const int pinDigitalMQ2 = 35; // DOUT
const int ledAlarma = 2; // LED conectado al pin GPIO 2 (opcional)

const float R0 = 9.83; // Valor aproximado en aire limpio para alcohol
const float m = -0.66; // Pendiente curva alcohol
const float b = 1.13;  // Intersección curva alcohol

void setup() {
  Serial.begin(115200);
  pinMode(pinDigitalMQ2, INPUT);
  pinMode(ledAlarma, OUTPUT);
  digitalWrite(ledAlarma, LOW);
  Serial.println("Detección de alcohol usando AOUT y DOUT (MQ-2 + ESP32)");
}

void loop() {
  // Analógica: cálculo de ppm
  float Rs = calcularRs();
  float ratio = Rs / R0;
  float ppm = pow(10, (log10(ratio) - b) / m);

  Serial.print("Alcohol estimado: ");
  Serial.print(ppm);
  Serial.println(" ppm");

  // Digital: alarma si el umbral de gas se supera
  int estadoDOUT = digitalRead(pinDigitalMQ2);
  if (estadoDOUT == LOW) {
    Serial.println("⚠️ ¡Alarma! Nivel de alcohol supera el umbral.");
    digitalWrite(ledAlarma, HIGH);
  } else {
    digitalWrite(ledAlarma, LOW);
  }

  Serial.println("---------------------------");
  delay(2000);
}

float calcularRs() {
  int adc = analogRead(pinAnalogMQ2);
  float voltage = adc * 3.3 / 4095.0;
  const float RL = 10.0;

  if (voltage <= 0.01) voltage = 0.01;
  float Rs = RL * (3.3 - voltage) / voltage;
  return Rs;
}