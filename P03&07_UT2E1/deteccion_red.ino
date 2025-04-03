#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;

int pitch;
int roll;
////////////VARIABLES RED NEURONAL///////
const int node =2;
const int input=2;
const int outputs =2;
float P[input][1];
float W1[node][input]= {{1.5554	, 1.2909},
                        {2.9279	,-0.16395}};
float b1[node][1]={{0.99526},
                   {2.5019}};
float a1[node][1];

float W2[outputs][node]={{-2.6185	, 1.9677},
                          {1.4156	, 4.4296}};
float b2[outputs][1]={{0.094262},
                       {-0.4239}};
float a2[outputs][1];
float aux=0.0;
int maxValue=1023;
int minValue=0;

void setup (){
Serial.begin(9600);
  Wire.begin(21, 22); // SDA en GPIO 21, SCL en GPIO 22

    Serial.println("Iniciando MPU6050...");

    while(!mpu.begin())
{
  delay(500);
}


    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}
void loop()
{  
////PRIMERA CAPA DE ENTRADA///
	  sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    // Cálculo de ángulos de inclinación
    roll  = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / M_PI;
    pitch = atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + accel.acceleration.z * accel.acceleration.z)) * 180.0 / M_PI;

pitch=map(pitch,-180,180,0,1023);
roll=map(roll,-180,180,0,1023);
P[0][0]=dataNormalized((float) pitch,minValue,maxValue);
P[1][0]=dataNormalized((float) roll,minValue,maxValue);

////////SEGUNDA CAPA/////////
for(int i=0; i<node; i++){
aux=0.0;
for(int j=0; j<input; j++){
aux=aux+W1[i][j]*P[j][0];
}
a1[i][0]=tansig(aux+b1[i][0]);
}

/////////TERCERA ETAPA/////
for(int i=0; i<outputs; i++){
  aux = 0.0;
for(int j=0; j<node; j++){
aux=aux+W2[i][j]*a1[j][0];
}
a2[i][0]=tansig(aux+b2[i][0]);
}

if (a2[0][0]<=0 && a2[1][0]<=0){
  Serial.println("Atras");
  }
  else if(a2[0][0]<=0 && a2[1][0]>0){
      Serial.println("izquierda");
      }
      else if(a2[0][0]>0 && a2[1][0]<=0){
        Serial.println("enfrente");
      }
      else
      {
        Serial.println("derecha");
        }
    delay(500);
  }

float tansig(float n)
{
  float a=exp(n);
  float b=exp(-n);
  return (a-b)/(a+b);
}

//////FUNCION PARA NORMALIZAR///////
float dataNormalized(int inputData, int minData, int maxData)
{
  float valueNorm;
  valueNorm = 2.0*(inputData-minData)/(maxData-minData)-1.0;
  return valueNorm;
}					   
