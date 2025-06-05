//Codigo sin filtro 
// Pines usados
const int pinAnalogMQ2 = 34;  // AOUT
const int pinDigitalMQ2 = 35; // DOUT

void setup() {
  Serial.begin(115200);
  pinMode(pinDigitalMQ2, INPUT);
}

void loop() {
  int valorAnalogico = analogRead(pinAnalogMQ2);   // Lectura analógica
  int valorDigital = digitalRead(pinDigitalMQ2);   // Lectura digital

  Serial.print("Valor analógico: ");
  Serial.print(valorAnalogico);
  Serial.print(" | Valor digital: ");
  Serial.println(valorDigital ? "GAS DETECTADO" : "AIRE LIMPIO");

  delay(1000); // Esperar 1 segundo
}