//Basiclly I grabbed some code from the RGB lab from project 1 and applied the same principle of the light changing as the servo moves.

#include <Servo.h>          //include the servo library

int potPosition;           //this variable will store the position of the potentiometer
int servoPosition;         //the servo will move to this position
int RedPin = 10;
int GreenPin = 11;
int BluePin = 12;
int threshold = 900;

Servo myservo;              //create a servo object

void setup() {
  Serial.begin(9600); 
  myservo.attach(9);        //tell the servo object that its servo is plugged into pin 9
  pinMode(RedPin,OUTPUT);
  pinMode(GreenPin,OUTPUT);
  pinMode(BluePin,OUTPUT);
}

void loop() {

  
  
  potPosition = analogRead(A0);                     //use analog read to measure the position of the potentiometer (0-1023)
  
  servoPosition = map(potPosition, 0,1023,20,160);  //convert the potentiometer number to a servo position from 20-160
                                                    //Note: its best to avoid driving the little SIK servos all the 
                                                    //way to 0 or 180 degrees it can cause the motor to jitter, which is bad for the servo.
  
  myservo.write(servoPosition);                      //move the servo to the 10 degree position
  
  if(servoPosition < threshold){         
    if(potPosition > 0 && potPosition <= 150)
      red();
    if(potPosition > 150 && potPosition <= 300)
      orange();
    if(potPosition > 300 && potPosition <= 450)
      yellow(); 
    if(potPosition > 450 && potPosition <= 600)
      green();
    if(potPosition > 600 && potPosition <= 750)
      cyan();
    if(potPosition > 750 && potPosition <= 900)
      blue(); 
    if(potPosition > 900)
      magenta();  
  } 
  else {                                //if it isn't dark turn the LED off
    
    turnOff();                            //call the turn off function
    
  }  
  
  delay(100);         
}

void red (){
    analogWrite(RedPin, 100);
    analogWrite(GreenPin, 0);
    analogWrite(BluePin, 0);
}
void orange (){
    
    analogWrite(RedPin, 100);
    analogWrite(GreenPin, 50);
    analogWrite(BluePin, 0);
}
void yellow (){
    
    analogWrite(RedPin, 100);
    analogWrite(GreenPin, 100);
    analogWrite(BluePin, 0);
}
void green (){
    
    analogWrite(RedPin, 0);
    analogWrite(GreenPin, 100);
    analogWrite(BluePin, 0);
}
void cyan (){
    
    analogWrite(RedPin, 0);
    analogWrite(GreenPin, 100);
    analogWrite(BluePin, 100);
}
void blue (){
    
    analogWrite(RedPin, 0);
    analogWrite(GreenPin, 0);
    analogWrite(BluePin, 100);
}
void magenta (){
    
    analogWrite(RedPin, 100);
    analogWrite(GreenPin, 0);
    analogWrite(BluePin, 100);
}
void turnOff (){
  
    analogWrite(RedPin, 0);
    analogWrite(GreenPin, 0);
    analogWrite(BluePin, 0);
}
