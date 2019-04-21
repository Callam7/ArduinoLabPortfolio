#define MPHONE A0//defines which analog to feed through

int sig = 0;//sets up variable to adjust the sensitivity of the microphone
void setup() {
  pinMode(MPHONE, INPUT);
  pinMode (13, OUTPUT);
  pinMode (12, OUTPUT);
  pinMode (11, OUTPUT);
  pinMode (10, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  sig = analogRead(MPHONE) * 50; //used to increase the microphone's sensitivity as well as variable range

//Red
//Lights up during timid levels of sound
 if (sig > 1 && sig < 450){
  digitalWrite(13, HIGH);
  delay(200);
  }
  else {
    digitalWrite(13, LOW);
    delay(200);
    }
    
//Green
//Usually should light up during average sound levels
 if (sig > 450 && sig < 700){
  digitalWrite(12, HIGH);
  delay(200);
  } 
  else {
    digitalWrite(12, LOW);
    delay(200);
    }
    
//Blue
//Will light up during above average sound levels (i.e: crowded building, construction, etc)    
 if (sig > 700 && sig < 950){
  digitalWrite(11, HIGH);
  delay(200);
  } 
  else {
    digitalWrite(11, LOW);
    delay(200);
    }
    
//Yellow
//Will only light up if sound levels are extreme.
 if (sig > 950){
  digitalWrite(10, HIGH);
  delay(200);
  } 
  else {
    digitalWrite(10, LOW);
    delay(200);
    }
  Serial.println(analogRead(MPHONE) * 50, DEC);
  delay(200);
}
