#include <Arduino.h>
#include <U8g2lib.h>

#define tempZero 3
#define tempOne 1
#define tempTwo 2
#define mosfetOne 5
#define mosfetTwo 6

int readZero, readOne, readTwo, differentialOne, differentialTwo, PWMOne, PWMTwo = 0;
String debugZero = "Pocket: ";
String debugOne = "    Read One: ";
String debugTwo = "    Read Two: ";
String debugThree = "Differential One: ";
String debugFour = "    PWM One: ";
String debugFive = "    Differential Two: ";
String debugSix = "    PWM Two: ";

float R1 = 10000;
float logR2, R2;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {
  pinMode(tempZero, INPUT);
  pinMode(tempOne, INPUT);
  pinMode(tempTwo, INPUT);

  pinMode(mosfetOne, OUTPUT);
  pinMode(mosfetTwo, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  readZero = pocketMin(analogRead(tempZero));
  readOne = analogRead(tempOne);
  readTwo = analogRead(tempTwo);
  differentialOne = readZero - readOne;
  differentialTwo = readZero - readTwo;
  // no differnace, no power. 15 °F differance full power.
  PWMOne = PWMClamp(map(differentialOne, 0, 80, 0, 255));
  PWMTwo = PWMClamp(map(differentialTwo, 0, 80, 0, 255));
  analogWrite(mosfetOne, PWMOne);
  analogWrite(mosfetTwo, PWMTwo);
  debug();
  delay(500);
}

int pocketMin(int temp){ // alt 248
  if (temp <= 480)
    return 480;
  else 
    return temp;
}

int PWMClamp(int PWM){
  if(PWM >= 255)
    return 255;
   else if (PWM <= 0)
    return 0;
   else
    return PWM;
}
void debug(){

  Serial.print(debugZero + readZero);
  Serial.print(debugOne + readOne);
  Serial.println(debugTwo + readTwo);
  Serial.print(debugThree + differentialOne);
  Serial.print(debugFour + PWMOne);
  Serial.print(debugFive + differentialTwo);
  Serial.println(debugSix + PWMTwo);
  
  Serial.print(convertTemp(readZero));
  Serial.println(" °F");
  
}

// 1 incriment = 0.18 degrees
// 265 = 32F 300 = 38F 401 = 56F 480 = 71F
float convertTemp(int Vo) {
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  float T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0;
  return T;
}
