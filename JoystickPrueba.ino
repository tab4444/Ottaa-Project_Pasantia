#include "Mouse.h"

int yPin = A2;
int xPin = A1;

//Switch click change
int btn1Pin = 10;
bool btn1Pressed;

//sensibility
int sensPin = A3;

//Switch on/off
int btn2Pin = 16;
bool btn2Pressed;

//Switch click
int btnClickPin = 14;
bool btnClickPressed;

bool enableMouse = false;

int yZero, xZero;
int yValue, xValue;
int sensValue;
int yMov, xMov;
int mouseClick = 0;
int stateClick = 0;

int smoothing=1;

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
    
    sensValue = getValue(sensPin, 0, 0, 1023, 250, 400);

    xMov=calcMov(xValue,-10,10,sensValue);  
    yMov=calcMov(yValue,-10,10,sensValue);  
    
    movMouse(false, "Valores: ",-xMov,yMov);

  }
    doClick();
  
  if(checkClickBtn(digitalRead(btn2Pin), LOW)){
      enableMouse = !enableMouse;
    }

  if(checkClickBtn(digitalRead(btn1Pin), LOW)){
    stateClick++;
    Serial.println("State is: " + stateClick);
  }
  
}

int posValues(int value, int smoothing, int realValue){
  if(smoothing != 0){
     return (realValue - value) / smoothing;
    }
    return realValue;
  }

void doClick(){
  switch(stateClick) {
    case 1: 
      //click izq
        if(checkClickBtn(digitalRead(btnClickPin), LOW) && !Mouse.isPressed()){
          Mouse.press();
          Serial.println("Click izq");
        }
        else if(!checkClickBtn(digitalRead(btnClickPin), LOW) && Mouse.isPressed()){
          Mouse.release(); 
        }
          
      break;
    case 2:
      //click der
      if(checkClickBtn(digitalRead(btnClickPin), LOW) && !Mouse.isPressed(MOUSE_RIGHT)){
        Mouse.press(MOUSE_RIGHT);
        Serial.println("Click der");
      }
      else if(!checkClickBtn(digitalRead(btnClickPin), LOW) && Mouse.isPressed(MOUSE_RIGHT)){
        Mouse.release(MOUSE_RIGHT); 
      }
      
      break;
    case 3:
      //scroll down
      if(checkClickBtn(digitalRead(btnClickPin), LOW)){
        Mouse.move(0,0,-1);
        Serial.println("Scroll Down");
      }
      
      break;
    case 4:
      //scroll up
      if(checkClickBtn(digitalRead(btnClickPin), LOW) && !Mouse.isPressed()){
        Mouse.move(0,0,1);
        Serial.println("Scroll Up");
      }
 
      break;
    default:
      //desactivado
      stateClick = 0;
      break;
  }  
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
