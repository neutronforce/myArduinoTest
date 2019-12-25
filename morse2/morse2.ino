#include <RGBmatrixPanel.h>

#define CLK  8
#define OE   9
#define LAT  10
#define DOTB  11
#define DASHB 12
#define JOYB 13
#define A    A0
#define B    A1
#define C    A2
#define D    A3
#define YB    A4
#define XB    A5

#define USHRT_MAX 65535
#define PAUSE 750
#define DELAY 50
#define DOT '.'
#define DASH '-'
#define NIL '\0'
#define MAX_MORSE 5
#define MAX_SCREEN 20

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

char morse[MAX_MORSE];
char text[MAX_SCREEN];
uint8_t mPos = 0;
uint8_t sPos = 0;
unsigned long signalStart = 0;
bool dotWasPushed = false;
bool dashWasPushed = false;

void setup() {
  //Serial.begin(9600);
  matrix.begin();
  matrix.setTextSize(1);
  matrix.setTextWrap(true);
  clearScreen();

  pinMode(DOTB, INPUT_PULLUP);
  pinMode(DASHB, INPUT_PULLUP);
  pinMode(JOYB, INPUT_PULLUP);

  digitalWrite(DOTB, HIGH);
  digitalWrite(DASHB, HIGH);
  digitalWrite(JOYB, HIGH);
}

void loop() {
  checkMorse();
}

void clearScreen() {
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setCursor(0, 0);
  sPos = 0;
  for (int i = 0; i < MAX_SCREEN; i++) {
    text[i] = ' ';
  }
  clearMorse();
}

void clearMorse() {
  for (int i = 0; i < MAX_MORSE; i++) {
    morse[i] = NIL;
  }
}

void checkMorse() {

  boolean dotPushed = (LOW == digitalRead(DOTB));
  boolean dashPushed = (LOW == digitalRead(DASHB));
  //  if (dotPushed) Serial.println(" .dot btn pushed. ");
  //  if (dashPushed) Serial.println(" -dash btn pushed- ");

  if ((dotPushed && dotWasPushed) || (dashPushed && dashWasPushed)) {
    //    Serial.println("button still pushed");
  }
  else if ((!dotPushed && dotWasPushed) || (!dashPushed && dashWasPushed)) {
    //    Serial.println("button released");
    if ((millis() - signalStart) > DELAY) {
      if (dotWasPushed) {
        //        Serial.println("DOT");
        morse[mPos++] = DOT;
      }
      else if (dashWasPushed) {
        //        Serial.println("DASH");
        morse[mPos++] = DASH;
      }
    }
    //Serial.print("Morse:" ); Serial.println(morse);
    //Serial.print("mPos:" ); Serial.println(mPos);
  }
  else if ((dotPushed && !dotWasPushed) || (dashPushed && !dashWasPushed))
  {
    //Serial.println("button pushed");
    signalStart = millis();
  }
  else if ((!dotPushed && !dotWasPushed) && (!dashPushed && !dashWasPushed))
  {
    if (mPos == MAX_MORSE || (((millis() - signalStart) > PAUSE ) && (mPos > 0)))
    {
      readMorse(morse);
      mPos = 0;
      clearMorse();
      //Serial.print("Morse after clearing:" ); Serial.println(morse);
    }
  }
  dotWasPushed = dotPushed;
  dashWasPushed = dashPushed;
}

void appendChar(char c) {
  if (sPos == MAX_SCREEN) {
    clearScreen();
  }
  //Serial.print("Append to screen: ");  Serial.println(c);
  matrix.setTextColor(matrix.ColorHSV(random(0, 1535), 255, 255, true));
  matrix.print(c);
  text[sPos++] = c;
}

void readMorse(char str[])
{
  //Serial.print("Decoding: ");
  //Serial.println(str);
  if (str[0] == DOT && str[1] == DASH && str[2] == NIL && str[3] == NIL && str[4] == NIL) {
    appendChar('A');
  }
  else if (str[0] == DASH && str[1] == DOT && str[2] == DOT && str[3] == DOT && str[4] == NIL) {
    appendChar('B');
  }
  else if (str[0] == DASH && str[1] == DOT && str[2] == DASH && str[3] == DOT && str[4] == NIL) {
    appendChar('C');
  }
  else if (str[0] == DASH && str[1] == DOT && str[2] == DOT && str[3] == NIL && str[4] == NIL) {
    appendChar('D');
  }
  else if (str[0] == DOT && str[1] == NIL && str[2] == NIL && str[3] == NIL && str[4] == NIL) {
    appendChar('E');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DASH && str[3] == DOT && str[4] == NIL) {
    appendChar('F');
  }
  else if (str[0] == DASH && str[1] == DASH && str[2] == DOT && str[3] == NIL && str[4] == NIL) {
    appendChar('G');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DOT && str[3] == DOT && str[4] == NIL) {
    appendChar('H');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == NIL && str[3] == NIL && str[4] == NIL) {
    appendChar('I');
  }
  else if (str[0] == DOT && str[1] == DASH && str[2] == DASH && str[3] == DASH && str[4] == NIL) {
    appendChar('J');
  }
  else if (str[0] == DASH && str[1] == DOT && str[2] == DASH && str[3] == NIL && str[4] == NIL) {
    appendChar('K');
  }
  else if (str[0] == DOT && str[1] == DASH && str[2] == DOT && str[3] == DOT && str[4] == NIL) {
    appendChar('L');
  }
  else if (str[0] == DASH && str[1] == DASH && str[2] == NIL && str[3] == NIL && str[4] == NIL) {
    appendChar('M');
  }
  else if (str[0] == DASH && str[1] == DOT && str[2] == NIL && str[3] == NIL && str[4] == NIL) {
    appendChar('N');
  }
  else if (str[0] == DASH && str[1] == DASH && str[2] == DASH && str[3] == NIL && str[4] == NIL) {
    appendChar('O');
  }
  else if (str[0] == DOT && str[1] == DASH && str[2] == DASH && str[3] == DOT && str[4] == NIL) {
    appendChar('P');
  }
  else if (str[0] == DASH && str[1] == DASH && str[2] == DOT && str[3] == DASH && str[4] == NIL) {
    appendChar('Q');
  }
  else if (str[0] == DOT && str[1] == DASH && str[2] == DOT && str[3] == NIL && str[4] == NIL) {
    appendChar('R');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DOT && str[3] == NIL && str[4] == NIL) {
    appendChar('S');
  }
  else if (str[0] == DASH && str[1] == NIL && str[2] == NIL && str[3] == NIL && str[4] == NIL) {
    appendChar('T');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DASH && str[3] == NIL && str[4] == NIL) {
    appendChar('U');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DOT && str[3] == DASH && str[4] == NIL) {
    appendChar('V');
  }
  else if (str[0] == DOT && str[1] == DASH && str[2] == DASH && str[3] == NIL && str[4] == NIL) {
    appendChar('W');
  }
  else if (str[0] == DASH && str[1] == DOT && str[2] == DOT && str[3] == DASH && str[4] == NIL) {
    appendChar('X');
  }
  else if (str[0] == DASH && str[1] == DOT && str[2] == DASH && str[3] == DASH && str[4] == NIL) {
    appendChar('Y');
  }
  else if (str[0] == DASH && str[1] == DASH && str[2] == DOT && str[3] == DOT && str[4] == NIL) {
    appendChar('Z');
  }
  else if (str[0] == DOT && str[1] == DASH && str[2] == DASH && str[3] == DASH && str[4] == DASH) {
    appendChar('1');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DASH && str[3] == DASH && str[4] == DASH) {
    appendChar('2');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DOT && str[3] == DASH && str[4] == DASH) {
    appendChar('3');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DOT && str[3] == DOT && str[4] == DASH) {
    appendChar('4');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DOT && str[3] == DOT && str[4] == DOT) {
    appendChar('5');
  }
  else if (str[0] == DASH && str[1] == DOT && str[2] == DOT && str[3] == DOT && str[4] == DOT) {
    appendChar('6');
  }
  else if (str[0] == DASH && str[1] == DASH && str[2] == DOT && str[3] == DOT && str[4] == DOT) {
    appendChar('7');
  }
  else if (str[0] == DASH && str[1] == DASH && str[2] == DASH && str[3] == DOT && str[4] == DOT) {
    appendChar('8');
  }
  else if (str[0] == DASH && str[1] == DASH && str[2] == DASH && str[3] == DASH && str[4] == DOT) {
    appendChar('9');
  }
  else if (str[0] == DASH && str[1] == DASH && str[2] == DASH && str[3] == DASH && str[4] == DASH) {
    appendChar('0');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DASH && str[3] == DASH && str[4] == NIL) {
    appendChar(' ');
  }
}
