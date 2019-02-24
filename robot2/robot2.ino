#include <Servo.h>

#define RT 3

#define DFWD 4
#define DREV 5
#define DRV 6
#define WFWD 7
#define WREV 8
#define WPN 9
#define TURN 10
#define RD 14
#define RC 15
#define RB 16
#define RA 17

const int NONE = 0;
const int FRONT = 2;
const int FRONT_ATTACK = 4;
const int BACK = 6;
const int BACK_ATTACK = 8;

int drive = 0;
int drivePrev = 0;
int pos = 0;
int posPrev = 0;

Servo myservo;
 
void setup() {
  Serial.begin(9600);
  pinMode(DRV,OUTPUT);
  pinMode(DFWD,OUTPUT);
  pinMode(DREV,OUTPUT);
  pinMode(WPN,OUTPUT);
  pinMode(WFWD,OUTPUT);
  pinMode(WREV,OUTPUT);
  attachInterrupt(1, readRemote, RISING); 
  pinMode(RA, INPUT);
  pinMode(RB, INPUT);
  pinMode(RC, INPUT);
  pinMode(RD, INPUT);
}

void loop() {
  if(pos != posPrev){
    posPrev = pos;
    myservo.attach(TURN);
    myservo.write(pos); 
    delay(500);
    Serial.print("Servo at: ");
    Serial.println(myservo.read());
    myservo.detach(); 
  }

  if(drive != drivePrev){
    drivePrev = drive;
    if(drive == NONE){
      digitalWrite(DRV, LOW);
      digitalWrite(DREV, LOW);
      digitalWrite(DFWD, LOW);  
      digitalWrite(WPN, LOW);
      digitalWrite(WREV, LOW);
      digitalWrite(WFWD, LOW)
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
      else {
        digitalWrite(WPN, HIGH);
        if(drive == FRONT_ATTACK){
          digitalWrite(DREV, LOW);
          digitalWrite(DFWD, HIGH);  
          digitalWrite(WREV, LOW);
          digitalWrite(WFWD, HIGH);  
        }
        else if(drive == BACK_ATTACK){
          digitalWrite(DREV, HIGH);
          digitalWrite(DFWD, LOW);
          digitalWrite(WREV, LOW);
          digitalWrite(WFWD, HIGH);    
        } 
      }
    }
  }
}

void readRemote(){
  if(digitalRead(RC) == HIGH){
    Serial.println("BUTTON C");
    if(drive == NONE){
      drive = FRONT;
    }
    else if(drive == BACK || drive == BACK_ATTACK) {
      drive = NONE;
    }
    else if(drive == FRONT){
      drive = FRONT_ATTACK;
    }
    else if(drive == FRONT_ATTACK){
      drive = FRONT;
    }
  }
  else if(digitalRead(RD) == HIGH){
    Serial.println("BUTTON D");
    if(drive == NONE){
      drive = BACK;
    }
    else if(drive == FRONT || drive == FRONT_ATTACK) {
      drive = NONE;
    }
    else if(drive == BACK){
      drive = BACK_ATTACK;
    }
    else if(drive == BACK_ATTACK){
      drive = BACK;
    }
  }
  else if(digitalRead(RB) == HIGH){
    Serial.println("BUTTON B");
    pos = 0;
  }
  else if(digitalRead(RA) == HIGH){
    Serial.println("BUTTON A");
    pos = 180;
  }
  Serial.print(drive);
  Serial.print(" ");
  Serial.println(pos);
}
