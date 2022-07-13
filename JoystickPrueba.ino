#include "Mouse.h"

int yPin = A2;
int xPin = A1;
int btn1Pin = 10;
int lastBtn1Press;
bool btn1Pressed;
int sensPin = A3;

int btn2Pin = 16;
int lastBtn2Press;
bool btn2Pressed;

bool enableMouse = false;

int yZero, xZero;
int yValue, xValue;
int sensValue;
int yMov, xMov;
int mouseClick = 0;

int smoothing=0;

void setup() {

  Serial.begin(9600);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(sensPin, INPUT);
  digitalWrite(btn1Pin, HIGH);
  delay(1000);
  yZero = analogRead(yPin);
  xZero = analogRead(xPin);
  xValue = 0;
  yValue = 0;

  Mouse.begin();
}

void loop() {
  if(enableMouse){
    yValue = posValues(yValue, smoothing ,getValue(yPin, yZero));
    xValue = posValues(xValue, smoothing ,getValue(xPin, xZero));
    
    sensValue = getValue(sensPin, 0, 0, 1023, 11, 200);
    //sensValue = 100;
    xMov=calcMov(xValue,-10,10,sensValue);  
    yMov=calcMov(yValue,-10,10,sensValue);  
  
    movMouse(false, "proa",-xMov,yMov);
    if(checkClickBtn(digitalRead(btn1Pin), LOW) && !Mouse.isPressed()){
      Mouse.press();
    }
    else if(!checkClickBtn(digitalRead(btn1Pin), LOW) && Mouse.isPressed()){
      Mouse.release(); 
      }
  }
  if(checkClickBtn(digitalRead(btn2Pin), LOW)){
      enableMouse = !enableMouse;
      lastBtn2Press = millis();
    }
  
}

int posValues(int value, int smoothing, int realValue){
  if(smoothing != 0){
     return (realValue - value) / smoothing;
    }
    return realValue;
  }

boolean checkClickBtn(int btnValue, int activationValue){
  if(btnValue == activationValue ){
    return true;
  }
  return false;
}

int movMouse(bool senSerial, String indicator,int xMov,int yMov){
  if(senSerial){
     Serial.println(indicator+ "," + String (xMov) +","+String(yMov));
    }
    else{
      Mouse.move(xMov, yMov, 0);
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
