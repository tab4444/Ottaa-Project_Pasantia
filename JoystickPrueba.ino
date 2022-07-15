#include "Mouse.h"

//Joystick_Pin
int yPin = A2;
int xPin = A1;

//Switch click change
int btn1Pin = 10;
bool btn1Pressed = false;

//sensibility
int sensPin = A3;

//Switch on/off
int btn2Pin = 16;
bool btn2Pressed = false;

//Switch click
int btnClickPin = 14;

bool enableMouse = false;

//Joystick_Movement
int yZero, xZero;
int yValue, xValue;
int sensValue;
int yMov, xMov;
int mouseClick = 0;
int stateClick = 0;

int smoothing = 1;

//Leds
int ledPin1 = 5;
int ledPin2 = 6;
int ledPin3 = 7;
int ledPin4 = 8;
int ledPin5 = 9;

void setup() {

  Serial.begin(9600);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);
  pinMode(sensPin, INPUT);
  //digitalWrite(btn1Pin, HIGH);
  delay(1000);
  yZero = analogRead(yPin);
  xZero = analogRead(xPin);
  xValue = 0;
  yValue = 0;

   pinMode(ledPin1 , OUTPUT);
   pinMode(ledPin2 , OUTPUT);
   pinMode(ledPin3 , OUTPUT);
   pinMode(ledPin4 , OUTPUT);
   pinMode(ledPin5 , OUTPUT);

  digitalWrite(ledPin1 , LOW);
  digitalWrite(ledPin2 , LOW);
  digitalWrite(ledPin3 , LOW);
  digitalWrite(ledPin4 , LOW);
  digitalWrite(ledPin5 , LOW);

  Mouse.begin();
}

void loop() {
  //movimiento del joystick
  if(enableMouse){
    digitalWrite(ledPin1 , HIGH);
    
    yValue = posValues(yValue, smoothing ,getValue(yPin, yZero));
    xValue = posValues(xValue, smoothing ,getValue(xPin, xZero));
    
    sensValue = getValue(sensPin, 0, 0, 1023, 250, 400);

    xMov=calcMov(xValue,-10,10,sensValue);  
    yMov=calcMov(yValue,-10,10,sensValue);  
    
    movMouse(false, "Valores: ",-xMov,yMov);

  }
    doClick();
  
  if(checkClickBtn(digitalRead(btn2Pin), LOW) && !btn2Pressed){
      digitalWrite(ledPin1 , LOW);
      enableMouse = !enableMouse;
      Serial.println("Enable: " + String(enableMouse));
      btn2Pressed = true;
      delay(150);
  } else if (checkClickBtn(digitalRead(btn2Pin), HIGH) && btn2Pressed){
    btn2Pressed = false;
    delay(150);
  }

  if(checkClickBtn(digitalRead(btn1Pin), LOW) && !btn1Pressed){
    stateClick++;
    Serial.println("State is: " + String(stateClick));
    btn1Pressed = true;
    delay(150);
  } else if (checkClickBtn(digitalRead(btn1Pin), HIGH) && btn1Pressed){
    btn1Pressed = false;
    delay(150);
  }
  
}


//smoothing
int posValues(int value, int smoothing, int realValue){
  if(smoothing != 0){
     return (realValue - value) / smoothing;
    }
    return realValue;
  }

  
//tipos de clicks
void doClick(){
  switch(stateClick) { 
    case 1: 
    Serial.println("state 1");
      digitalWrite(ledPin2 , HIGH);
      //click izq
        if(checkClickBtn(digitalRead(btnClickPin), LOW) && !Mouse.isPressed()){
          Mouse.press();
        }
        else if(!checkClickBtn(digitalRead(btnClickPin), LOW) && Mouse.isPressed()){
          Mouse.release(); 
          delay(150);
        }
          
      break;
    case 2:
    Serial.println("state 2");
      digitalWrite(ledPin3 , HIGH);
    Serial.println(digitalRead(ledPin3));
      //click der
      if(checkClickBtn(digitalRead(btnClickPin), LOW) && !Mouse.isPressed(MOUSE_RIGHT)){
        Mouse.press(MOUSE_RIGHT);
      }
      else if(!checkClickBtn(digitalRead(btnClickPin), LOW) && Mouse.isPressed(MOUSE_RIGHT)){
        Mouse.release(MOUSE_RIGHT); 
        delay(150);
      }
      
      break;
    case 3:
    Serial.println("state 3");
      digitalWrite(ledPin4 , HIGH);
      //scroll down
      if(checkClickBtn(digitalRead(btnClickPin), LOW)){
        Mouse.move(0,0,-40);
        delay(150);
      }
      
      break;
    case 4:
    Serial.println("state 4");
      digitalWrite(ledPin5 , HIGH);
      //scroll up
      if(checkClickBtn(digitalRead(btnClickPin), LOW)){
        Mouse.move(0,0,40);
        delay(150);
      }
 
      break;
    default:
      digitalWrite(ledPin2 , LOW);
      digitalWrite(ledPin3 , LOW);
      digitalWrite(ledPin4 , LOW);
      digitalWrite(ledPin5 , LOW);
      
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
