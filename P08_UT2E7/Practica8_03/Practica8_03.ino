// Pines de los sensores
const int sensor1Pin = 18;
const int sensor2Pin = 19;

// Variables de conteo
volatile unsigned long pulseCount1 = 0;
volatile unsigned long pulseCount2 = 0;

// Calibración (igual para ambos sensores)
const float calibrationFactor = 7.5;

// Volumen total
float totalLiters1 = 0.0;
float totalLiters2 = 0.0;

portMUX_TYPE mux1 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE mux2 = portMUX_INITIALIZER_UNLOCKED;

// Interrupciones
void IRAM_ATTR pulseCounter1() {
  portENTER_CRITICAL_ISR(&mux1);
  pulseCount1++;
  portEXIT_CRITICAL_ISR(&mux1);
}

void IRAM_ATTR pulseCounter2() {
  portENTER_CRITICAL_ISR(&mux2);
  pulseCount2++;
  portEXIT_CRITICAL_ISR(&mux2);
}

void setup() {
  Serial.begin(115200);
  pinMode(sensor1Pin, INPUT_PULLUP);
  pinMode(sensor2Pin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(sensor1Pin), pulseCounter1, RISING);
  attachInterrupt(digitalPinToInterrupt(sensor2Pin), pulseCounter2, RISING);
}

void loop() {
  static unsigned long lastTime = millis();
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= 1000) { // cada segundo
    // Leer y resetear pulsos
    portENTER_CRITICAL(&mux1);
    unsigned long pulses1 = pulseCount1;
    pulseCount1 = 0;
    portEXIT_CRITICAL(&mux1);

    portENTER_CRITICAL(&mux2);
    unsigned long pulses2 = pulseCount2;
    pulseCount2 = 0;
    portEXIT_CRITICAL(&mux2);

    // Calcular flujos
    float flow1_Lmin = pulses1 / calibrationFactor;
    float flow2_Lmin = pulses2 / calibrationFactor;

    float flow1_Lsec = flow1_Lmin / 60.0;
    float flow2_Lsec = flow2_Lmin / 60.0;

    // Acumular
    totalLiters1 += flow1_Lsec;
    totalLiters2 += flow2_Lsec;

    // Calcular promedio
    float avgFlow = (flow1_Lmin + flow2_Lmin) / 2.0;
    float diff = flow1_Lmin - flow2_Lmin;

    // Mostrar resultados
    Serial.println("=== FLUJO EN SERIE ===");
    Serial.print("Sensor 1: ");
    Serial.print(flow1_Lmin, 2);
    Serial.print(" L/min\t Total: ");
    Serial.print(totalLiters1, 3);
    Serial.println(" L");

    Serial.print("Sensor 2: ");
    Serial.print(flow2_Lmin, 2);
    Serial.print(" L/min\t Total: ");
    Serial.print(totalLiters2, 3);
    Serial.println(" L");

    Serial.print("Diferencia: ");
    Serial.print(diff, 2);
    

    lastTime = currentTime;
  }
}