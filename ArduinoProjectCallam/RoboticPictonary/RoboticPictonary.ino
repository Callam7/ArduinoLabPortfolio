//Version 2.0 of my project, where it is basiclly robotic pictonary!
//Although not the most functional game, it does respond very well to the dials on the etch a sketch

const int AIN1 = 13;           
const int AIN2 = 12;            
const int PWMA = 11;            

const int PWMB = 10;           
const int BIN2 = 9;           
const int BIN1 = 8;           

int switchPin = 7;             

const int drawTime = 20;      


const int turnTime = 8;        


String picDirection;           
String distance;               

/********************************************************************************/
void setup()
{
  pinMode(switchPin, INPUT_PULLUP);   

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial.begin(9600);           

  Serial.println("Enter a diretion followed by a distance.");
  Serial.println("f = forward, b = backward, r = turn right, l = turn left");
  Serial.println("Example command: f 50");
  Serial.println("Try to draw the following WITHOUT touching the etch a skecth, and just by using the above commands: ");
  Serial.println("Bunny Rabbit");
  Serial.println("Boat");
  Serial.println("Ball");
  Serial.println("Smile Face");
}

/********************************************************************************/
void loop()
{
  if(digitalRead(7) == LOW)
  {                                                     
    if (Serial.available() > 0)                         
    {
      picDirection = Serial.readStringUntil(' ');       
      distance = Serial.readStringUntil(' ');           
  
      Serial.print(picDirection);                       
      Serial.print(" ");                                
      Serial.println(distance.toInt());                 
  
      if(picDirection == "f")                                                   
      {
        rightMotor(200);                                
        leftMotor(200);                                 
        delay(drawTime * distance.toInt());            
        rightMotor(0);                                  
        leftMotor(0);                                   
      }
      else if(picDirection == "b")                     
      {
        rightMotor(-200);                               
        leftMotor(-200);                                
        delay(drawTime * distance.toInt());            
        rightMotor(0);                                  
        leftMotor(0);                                   
      }
      else if(picDirection == "r")                        
      {
        rightMotor(-200);                               
        leftMotor(255);                                 
        delay(turnTime * distance.toInt());             
        rightMotor(0);                                  
        leftMotor(0);                                   
      }
      else if(picDirection == "l")                    
      { 
        rightMotor(255);                                
        leftMotor(-200);                                
        delay(turnTime * distance.toInt());             
        rightMotor(0);                                  
        leftMotor(0);                                   
      }
    }
  }
  else
  {
     rightMotor(0);                                  
     leftMotor(0);                                   
  }
}
/********************************************************************************/
void rightMotor(int motorSpeed)                       
{
  if (motorSpeed > 0)                                 
  {
    digitalWrite(AIN1, HIGH);                         
    digitalWrite(AIN2, LOW);                          
  }
  else if (motorSpeed < 0)                            
  {
    digitalWrite(AIN1, LOW);                          
    digitalWrite(AIN2, HIGH);                         
  }
  else                                                
  {
    digitalWrite(AIN1, LOW);                          
    digitalWrite(AIN2, LOW);                          
  }
  analogWrite(PWMA, abs(motorSpeed));                 
}

/********************************************************************************/
void leftMotor(int motorSpeed)                        
{
  if (motorSpeed > 0)                                 
  {
    digitalWrite(BIN1, HIGH);                         
    digitalWrite(BIN2, LOW);                          
  }
  else if (motorSpeed < 0)                            
  {
    digitalWrite(BIN1, LOW);                          
    digitalWrite(BIN2, HIGH);                         
  }
  else                                                
  {
    digitalWrite(BIN1, LOW);                          
    digitalWrite(BIN2, LOW);                          
  }
  analogWrite(PWMB, abs(motorSpeed));                 
}
