const int sensorPin = 18;  // Pin del ESP32 donde conectas la señal del sensor
volatile unsigned int pulseCount = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED; // Para proteger acceso desde la ISR

void IRAM_ATTR countPulse() {
  portENTER_CRITICAL_ISR(&mux);
  pulseCount++;
  portEXIT_CRITICAL_ISR(&mux);
}

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, RISING);
}

void loop() {
  pulseCount = 0;  // Reinicia contador

  delay(1000);     // Espera 1 segundo (puedes cambiarlo a 2000 o 500 si deseas promediar más rápido)

  portENTER_CRITICAL(&mux);
  unsigned int pulses = pulseCount;
  portEXIT_CRITICAL(&mux);

  Serial.print("Pulsos por segundo: ");
  Serial.println(pulses);

  // Puedes mostrar también el caudal estimado si lo deseas:
  float flowLmin = pulses / 7.5;  // Usando factor típico
  Serial.print("Caudal estimado: ");
  Serial.print(flowLmin, 2);
  Serial.println(" L/min\n");

  // Espera antes de siguiente medición (opcional)
  delay(500);
}
