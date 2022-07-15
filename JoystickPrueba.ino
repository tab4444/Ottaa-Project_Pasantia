//libreria utilizada
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

//define el valor del smoothing
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
  
   //Definimos el estado de los Pines de los leds
   pinMode(ledPin1 , OUTPUT);
   pinMode(ledPin2 , OUTPUT);
   pinMode(ledPin3 , OUTPUT);
   pinMode(ledPin4 , OUTPUT);
   pinMode(ledPin5 , OUTPUT);
 
  //Apagamos todos los leds
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
    
    movMouse(-xMov,yMov);

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
    /*
  Función para configurar los distintos tipos de clicks
  Inputs:
  - None
  Descripción:
  define el tipo de click que vas a utilizar a oprimir el boton del joystick, ya sea boton izquierdo, derecho, scroll up y scroll down
  Outputs: 
  - None
  */    
  switch(stateClick) { 
    case 1://boton izquierdo
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
    case 2://boton derecho
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
    case 3://scroll down 
    Serial.println("state 3");
      digitalWrite(ledPin4 , HIGH);
      //scroll down
      if(checkClickBtn(digitalRead(btnClickPin), LOW)){
        Mouse.move(0,0,-40);
        delay(150);
      }
      
      break;
    case 4://scroll up
    Serial.println("state 4");
      digitalWrite(ledPin5 , HIGH);
      //scroll up
      if(checkClickBtn(digitalRead(btnClickPin), LOW)){
        Mouse.move(0,0,40);
        delay(150);
      }
 
      break;
    default:
    //apagaga los leds
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
  /* 
   Función que revisa si se hace click
   Inputs:
   - btnValue[int]: se pasa el valor del pin del botón
   - activationBlue[int]: se pasa el valor de energía del botón
   */
  if(btnValue == activationValue ){
    return true;
  }
  return false;
}

int movMouse(int xMov,int yMov){
  /*
  Función para mover el cursor en pantalla
  Inputs:
  - xMov[int]: movimiento en x
  - yMov[int]: movimiento en y
  Descripción:
  Tomando los valores de movimiento en "x" e "y", y usando la función Mouse, se mueve el cursor.
  Outputs: 
  - None
  */    
  Mouse.move(xMov, yMov, 0);
 }

int calcMov(int value, int minLimit, int maxLimit, int sens){
    /*
  Función para calcular el movimiento del cursor
  Inputs:
  - Value[int]:Recibe el movimiento de x o y
  - minLimit[int]: Minimo movimiento del cursor
   maxLimit[int]: Maximo movimiento del cursor
  - sens[int]:sensibilidad del cursor
  Descripción:
  Tomando los valores de movimiento en "x" e "y", calcula el rango de movimiento, para que sea comodo para el usuario
  - None
  */
   if(value > maxLimit || value < minLimit){
      return value / sens;
    }
    return 0;
  }

int calcMov(int value, int minLimit, int maxLimit, int sens, int defaultMov){
  /*
   Función para calcular el movimiento del cursor con smoothing
   Inputs:
   - value[int]: recibe el movimiento de x o y
   - minLimit[int]: mínimo movimiento permitido del cursor
   - maxLimit[int]: máximo movimiento permitido del cursor
   - sens[int]: nivel de la sensibilidad
   - defaultMov[int]: movimiento estandar del cursor
   Outputs:
   - En caso de que value sea mayor que maxLimit o menor a minLimit, se devuelve el valor dividido la sensibilidad
   - En caso de que lo anterior no suceda, se retorna defaultMov
  */
   if(value > maxLimit || value < minLimit){
      return value / sens;
    }
    return defaultMov;
  }

int getValue(int pin, int zero){
  /*
   Función para calcular la posición del cursor en pantalla
   Inputs:
   - pin[int]: valor del eje x o y
   - zero[int]: valor zero del eje x o y
   Outputs: se retorna el valor del pin, restado por zero
   */
  return analogRead(pin) - zero;
}

int getValue(int pin, int zero, int initMin, int initMax, int mapMin, int mapMax){
  /*
   Función para calcular el nivel de sensibilidad
   Inputs: 
   - pin[int]: valor del pin del potenciometro
   - zero[int]: valor zero del potenciometro
   - initMin[int]: valor mínimo inicial
   - initMax[int]: valor máximo inicial
   - mapMin[int]: valor mínimo de la sensibilidad
   - mapMax[int]; valor máximo de la sensibilidad
   Outputs:
   - Se retorna el nivel de la sensiblidad
   */
  return map(analogRead(pin), initMin, initMax, mapMin, mapMax) - zero;
}
