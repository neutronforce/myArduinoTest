#define RT 3
#define DRV 4
#define DREV 5
#define DFWD 6
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
 
void setup() {
  Serial.begin(9600);
  pinMode(DRV,OUTPUT);
  pinMode(DFWD,OUTPUT);
  pinMode(DREV,OUTPUT);
  attachInterrupt(1, readRemote, CHANGE); 
  pinMode(RA, INPUT);
  pinMode(RB, INPUT);
  pinMode(RC, INPUT);
  pinMode(RD, INPUT);
}

void loop() {
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
    
  }
}
