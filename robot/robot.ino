#include "IRremote.h"

int E1 = 5;  
int M1 = 4; //LEFT
int E2 = 6;                      
int M2 = 7; //RIGHT
int WP = 8; //FAN
int SPKR = 10; //PIEZO
int IR = 11; 

const int FL = 1;
const int FRONT = 2;
const int FR = 3;
const int LEFT = 4;
const int RIGHT = 6;
const int BACK = 8;

int lastMove = 0;
long moveTime = 0;

IRrecv irrecv(IR);    
decode_results results;    

void setup() {
  Serial.begin(9600);
  pinMode(M1, OUTPUT);   
  pinMode(M2, OUTPUT); 
  pinMode(WP, OUTPUT);
  pinMode(SP, OUTPUT);
  irrecv.enableIRIn();
  Serial.println("Ready"); 
}

void move(int pwmL, int pwmR, int left, int right) {
  moveTime = 50000L;
  digitalWrite(M1, left);
  digitalWrite(M2, right);
  analogWrite(E1, pwmL);
  analogWrite(E2, pwmR);
}

void stop() {
  analogWrite(E1, 0);
  analogWrite(E2, 0);
}

void moveFrontLeft(){
  lastMove = FL;
  move(125, 255, HIGH, HIGH);  
}

void moveForward(){
  lastMove = FRONT;
  move(255, 255, HIGH, HIGH);  //FORWARD 
}

void moveFrontRight(){
  lastMove = FR;
  move(255, 125, HIGH, HIGH);  
}

void moveLeft(){
  lastMove = LEFT;
  move(255, 255, LOW, HIGH); //LEFT
}

void moveRight(){
  lastMove = RIGHT;
  move(255, 255, HIGH, LOW);  //RIGHT    
}

void moveBack(){
  lastMove = BACK;
  move(255, 255, LOW, LOW);   //BACK   
}

void repeatLast(){
  switch(lastMove){
    case FL: moveFrontLeft(); break;
    case FRONT: moveForward(); break;
    case FR: moveFrontRight(); break;
    case BACK: moveBack(); break;
    case LEFT: moveLeft(); break;
    case RIGHT: moveRight(); break;
    default: break;
  }
}

void weaponOn(){
  analogWrite(SPKR, 128);
  digitalWrite(WP, HIGH);
}

void weaponOff(){
  lastMove = 0;
  moveTime = 0;
  digitalWrite(WP, LOW);
  digitalWrite(SPKR, LOW);
}

void translateIR() {
  Serial.println(results.value); 
  
  switch(results.value)  {
    case 0xFFA25D: 
      Serial.println("POWER"); 
      break;
    
    case 0xFF629D: 
      Serial.println("VOL+"); 
      break;
    case 0xFF22DD: 
      Serial.println("FAST BACK");    
      break;
    case 0xFF02FD: 
      Serial.println("PAUSE");    
      break;
    case 0xFFC23D: 
      Serial.println("FAST FORWARD");  
      break;
    case 0xFFE01F: 
     Serial.println("DOWN");    
     break;
    case 0xFFA857: 
      Serial.println("VOL-");
      break;
    case 0xFF906F:
      Serial.println("UP");    
      break;
    
    case 0xFF6897: 
    case 0x20DF08F7:
      Serial.println("0");
      weaponOff();    
      break;
    case 0xFF30CF: 
    case 0x20DF8877:
      Serial.println("1"); 
      moveFrontLeft();
      break;
    case 0xFF18E7: 
    case 0x20DF48B7:
      Serial.println("2");
      moveForward();    
      break;
    case 0xFF7A85: 
    case 0x20DFC837:
      Serial.println("3");  
      moveFrontRight();  
      break;
    case 0xFF10EF: 
    case 0x20DF28D7:
      Serial.println("4");
      moveLeft();    
      break;
    case 0xFF38C7: 
    case 0x20DFA857:
      Serial.println("5");
      weaponOn();  
      break;
    case 0xFF5AA5:
    case 0x20DF6897: 
      Serial.println("6");    
      moveRight();
      break;
    case 0xFF42BD: 
      Serial.println("7");    
      break;
    case 0xFF4AB5: 
    case 0x20DF18E7:
      Serial.println("8");    
      moveBack();
      break;
    case 0xFF52AD: 
      Serial.println("9");    
      break;
    case 0xFFFFFFFF: 
      Serial.println(" REPEAT");
      repeatLast();
      break;  
  default: 
    Serial.println(" other button ");
  }
}

void loop() {
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value  
  }
  else if(--moveTime <= 0){
     stop();
  }
}
