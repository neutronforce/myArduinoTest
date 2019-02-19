#include <Servo.h>

#define RT 2
#define TURN 3
#define DFWD 4
#define DREV 5
#define DRV 6
#define RD 7
#define RC 8
#define RB 9
#define RA 10

const int NONE = 0;
const int FRONT = 2;
const int LEFT = 4;
const int RIGHT = 6;
const int BACK = 8;

int drive = 0;
int pos = 90;

Servo myservo;
 
void setup() {
  Serial.begin(9600);
  pinMode(DRV,OUTPUT);
  pinMode(DFWD,OUTPUT);
  pinMode(DREV,OUTPUT);
  attachInterrupt(0, readRemote, RISING); 
  pinMode(RA, INPUT);
  pinMode(RB, INPUT);
  pinMode(RC, INPUT);
  pinMode(RD, INPUT);
 // myservo.attach(TURN);
  
}

void loop() {
  //myservo.write(pos);
  
  if(drive == NONE){
    digitalWrite(DRV, LOW);
    digitalWrite(DREV, LOW);
    digitalWrite(DFWD, LOW);  
  }
  else {
    digitalWrite(DRV, HIGH);
    if(drive == FRONT){
      digitalWrite(DREV, LOW);
      digitalWrite(DFWD, HIGH);  
    }
    else if(drive == BACK){
      digitalWrite(DREV, HIGH);
      digitalWrite(DFWD, LOW);  
    }
  }
}

void readRemote(){
  if(digitalRead(RC) == HIGH){
    Serial.println("BUTTON C");
    if(drive == NONE){
      drive = FRONT;
    }
    else if(drive == FRONT) {
      drive = NONE;
    }
  }
  else if(digitalRead(RD) == HIGH){
    Serial.println("BUTTON D");
    if(drive == NONE){
      drive = BACK;
    }
    else if(drive == BACK) {
      drive = NONE;
    }
  }
  else if(digitalRead(RA) == HIGH){
    Serial.println("BUTTON A");
    pos -= 30;  
    if(pos < 0) pos = 0;
  }
  else if(digitalRead(RB) == HIGH){
    Serial.println("BUTTON B");
    pos += 30;  
    if(pos > 180) pos = 180;
  }
  Serial.print(drive);
  Serial.print(" ");
  Serial.println(pos);
}
