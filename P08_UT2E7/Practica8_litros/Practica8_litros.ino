const int sensorPin = 18; // GPIO conectado a la salida del YF-S201

volatile unsigned long pulseCount = 0; // Pulsos del sensor
float calibrationFactor = 7.5;         // Pulsos por segundo por L/min

unsigned long lastTime = 0;
float totalLiters = 0.0;
float totalCubicMeters = 0.0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR pulseCounter() {
  portENTER_CRITICAL_ISR(&mux);
  pulseCount++;
  portEXIT_CRITICAL_ISR(&mux);
}

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounter, RISING);
  lastTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= 1000) { // Cada segundo
    portENTER_CRITICAL(&mux);
    unsigned long pulses = pulseCount;
    pulseCount = 0;
    portEXIT_CRITICAL(&mux);

    float flowRate_Lmin = pulses / calibrationFactor;
    float flowRate_Lsec = flowRate_Lmin / 60.0;

    totalLiters += flowRate_Lsec;
    totalCubicMeters = totalLiters / 1000.0;

    Serial.print("Flujo: ");
    Serial.print(flowRate_Lmin, 2);
    Serial.print(" L/min\tAcumulado: ");
    Serial.print(totalLiters, 3);
    Serial.print(" L\t");
    Serial.print(totalCubicMeters, 6);
    Serial.println(" m³");

    lastTime = currentTime;
  }
}

