#include "Mouse.h"

int yPin = A2;
int xPin = A1;
int btn1Pin = 6;
int lastBtn1Press;
bool btn1Pressed;
int sensPin = A5;

int yZero, xZero;
int yValue, xValue;
int sensValue;
int yMov, xMov;
int mouseClick = 0;

void setup() {

  Serial.begin(9600);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(btn1Pin, INPUT);
  pinMode(sensPin, INPUT);
  digitalWrite(btn1Pin, LOW);
  delay(1000);
  yZero = analogRead(yPin);
  xZero = analogRead(xPin);
}

void loop() {
  yValue = getValue(yPin, yZero);
  xValue = getValue(xPin, xZero);
  sensValue = getValue(sensPin, 0, 0, 1023, 11, 200);
  
  xMov=calcMov(xValue,-10,10,sensValue);  
  yMov=calcMov(yValue,-10,10,sensValue);  
  
  //movMouse(true, "proa",-xMov,yMov);
  Serial.println(digitalRead(btn1Pin));
  
  //if(checkClickBtn(digitalRead(btn1Pin), HIGH, lastBtn1Press, 100)){
  //  Serial.println("click,simple");
  //  lastBtn1Press = millis();
  //}
}

boolean checkClickBtn(int btnValue, int activationValue, int lastBtnPress, int btnPressLimit){
  if(btnValue == activationValue && millis()-lastBtnPress > btnPressLimit){
    return true;
  }
  return false;
}

int movMouse(bool senSerial, String indicator,int xMov,int yMov){
  if(senSerial){
     Serial.println(indicator+ "," + String (xMov) +","+String(yMov));
    }
    else{
      //aca vendria el movimiento del mouse
      }
  
  }

int calcMov(int value, int minLimit, int maxLimit, int sens){
   if(value > maxLimit || value < minLimit){
      return value / sens;
    }
    return 0;
  }

int calcMov(int value, int minLimit, int maxLimit, int sens, int defaultMov){
   if(value > maxLimit || value < minLimit){
      return value / sens;
    }
    return defaultMov;
  }

int getValue(int pin, int zero){
  return analogRead(pin) - zero;
}

int getValue(int pin, int zero, int initMin, int initMax, int mapMin, int mapMax){
  return map(analogRead(pin), initMin, initMax, mapMin, mapMax) - zero;
}
