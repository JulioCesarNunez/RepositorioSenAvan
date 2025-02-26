#include <SoftwareSerial.h>

const int RXPin = A3;  // GPS TX -> Arduino Pin 3
const int TXPin = A4;  // GPS RX -> Arduino Pin 4
const int GPSBaud = 9600;  // Velocidad del GPS

SoftwareSerial gpsSerial(RXPin, TXPin);  // Puerto serial para GPS
String gpsData = "";  // Almacena datos de la línea NMEA

void setup() {
    Serial.begin(9600);  // Comunicación con el PC
    gpsSerial.begin(GPSBaud);  // Comunicación con GPS
    Serial.println("Esperando señal GPS...");
}

void loop() {
    while (gpsSerial.available()) {
        char c = gpsSerial.read();  // Leer un carácter del GPS
        if (c == '\n') {  // Fin de línea
            if (gpsData.startsWith("$GPGGA")) {  // Si es un mensaje GPGGA
                procesarGPGGA(gpsData);  // Extraer información
            }
            gpsData = "";  // Limpiar para la siguiente línea
        } else {
            gpsData += c;  // Agregar el carácter a la línea actual
        }
    }
}

// Función para procesar datos del mensaje GPGGA
void procesarGPGGA(String nmea) {
    int comas[15];  // Índices de las comas en la línea NMEA
    int count = 0;

    // Buscar posiciones de las comas
    for (int i = 0; i < nmea.length(); i++) {
        if (nmea[i] == ',') {
            comas[count++] = i;
            if (count >= 15) break;  // No necesitamos más de 15 comas
        }
    }

    if (count < 9) return;  // Verificar que tenga suficientes datos

    // Extraer hora UTC (HHMMSS)
    String horaUTC = nmea.substring(comas[0] + 1, comas[1]);

    // Extraer latitud
    String lat = nmea.substring(comas[1] + 1, comas[2]);
    char latDir = nmea[comas[2] + 1];  // Norte o Sur

    // Extraer longitud
    String lon = nmea.substring(comas[3] + 1, comas[4]);
    char lonDir = nmea[comas[4] + 1];  // Este u Oeste

    // Extraer cantidad de satélites
    String satelites = nmea.substring(comas[6] + 1, comas[7]);

    // Extraer altitud
    String altitud = nmea.substring(comas[8] + 1, comas[9]);

    // Convertir coordenadas a grados decimales
    float latitud = convertirCoordenada(lat, true);
    if (latDir == 'S') latitud *= -1;  // Sur es negativo

    float longitud = convertirCoordenada(lon, false);
    if (lonDir == 'W') longitud *= -1;  // Oeste es negativo

    // Mostrar datos procesados
    Serial.println("----- Datos GPS -----");
    Serial.print("Hora UTC: ");
    Serial.println(horaUTC);
    Serial.print("Latitud: ");
    Serial.print(latitud, 6);
    Serial.print(" | Longitud: ");
    Serial.println(longitud, 6);
    Serial.print("Satélites: ");
    Serial.println(satelites);
    Serial.print("Altitud: ");
    Serial.print(altitud);
    Serial.println(" m");
    Serial.println("---------------------\n");
}

// Convierte formato NMEA a grados decimales (ahora detecta longitud y latitud)
float convertirCoordenada(String coordenada, bool esLatitud) {
    if (coordenada.length() < 6) return 0;  // Evitar errores en datos incompletos

    int numGrados = esLatitud ? 2 : 3;  // Latitud tiene 2 grados, Longitud tiene 3 grados
    float grados = coordenada.substring(0, numGrados).toFloat();  // Extraer los grados
    float minutos = coordenada.substring(numGrados).toFloat();  // Extraer los minutos

    return grados + (minutos / 60.0);  // Convertir a decimal
}
