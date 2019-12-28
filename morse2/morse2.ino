#include <RGBmatrixPanel.h>
#include <EEPROM.h>
#include "rabbit.h"

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
#define IMAGE_DELAY 3000
#define PAUSE 750
#define DELAY 50
#define DOT '.'
#define DASH '-'
#define NIL '\0'
#define MAX_HUE 1535
#define MAX_MORSE 6
#define MAX_SCREEN 20
#define LAST_SCREEN 4
#define FIRST_SCREEN 1
#define TEXT_MIN -127
#define MODE_BOOT 0
#define MODE_SCROLL 1
#define MODE_EDIT 2

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

char morse[MAX_MORSE];
char text[MAX_SCREEN];
uint8_t mPos = 0;
uint8_t sPos = 0;
int8_t textX = matrix.width();
uint8_t screen = FIRST_SCREEN;
uint8_t mode = MODE_BOOT;
unsigned long signalStart = 0;
bool dotWasPushed = false;
bool dashWasPushed = false;
bool modeWasPushed = false;
bool leftWasMoved = false;
bool rightWasMoved = false;
bool upWasMoved = false;
bool downWasMoved = false;

void setup() {
  //Serial.begin(9600);
  matrix.begin();
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  clearScreen();

  pinMode(DOTB, INPUT_PULLUP);
  pinMode(DASHB, INPUT_PULLUP);
  pinMode(JOYB, INPUT_PULLUP);

  digitalWrite(DOTB, HIGH);
  digitalWrite(DASHB, HIGH);
  digitalWrite(JOYB, HIGH);

  signalStart = millis() - IMAGE_DELAY;
}

void loop() {
  if (mode == MODE_BOOT) {
    if ((millis() - signalStart) > IMAGE_DELAY) {
      signalStart = millis();
      matrix.drawXBitmap(0, 0, img, 32, 32, matrix.ColorHSV(random(0, MAX_HUE), 255, 255, true));
    }
  }
  else if (mode == MODE_EDIT) {
    checkMorse();
    checkArrows();
  }
  else if (mode == MODE_SCROLL) {
    scrollSavedScreens();
  }
  checkMode();
}

void checkArrows() {
  int x = analogRead(XB);
  int y = analogRead(YB);

  bool leftMove = (x < 480);
  bool rightMove = (x > 544);
  bool upMove = (y < 412);
  bool downMove = (y > 612);

  if (!leftMove && leftWasMoved) {
    //Serial.println(F("backspace"));
    doBackSpace();
    upWasMoved = false;
    downWasMoved = false;
  }
  else if (!rightMove && rightWasMoved) {
    //Serial.println(F("space"));
    if (sPos < MAX_SCREEN) {
      appendChar(' ');
      clearMorse();
      //Serial.println(sPos);
    }
    upWasMoved = false;
    downWasMoved = false;
  }
  else if (!upMove && upWasMoved) {
    //screen up
    if (screen > FIRST_SCREEN) {
      screen--;
      loadScreen();
      //Serial.print(F("screen: ")); Serial.println(screen);
    }
  }
  else if (!downMove && downWasMoved) {
    //screen down
    if (screen < LAST_SCREEN) {
      screen++;
      loadScreen();
      //Serial.print(F("screen: ")); Serial.println(screen);
    }
  }

  leftWasMoved = leftMove;
  rightWasMoved = rightMove;
  upWasMoved = upMove;
  downWasMoved = downMove;
}

void checkMode() {
  bool modePushed = (LOW == digitalRead(JOYB));
  if (!modePushed && modeWasPushed) {
    //Serial.println(F("mode button released."));
    if (mode == MODE_EDIT || mode == MODE_BOOT) {
      //Serial.println(F("edit mode: true -> false."));
      if (mode == MODE_EDIT){
        saveScreen();
      }
      mode = MODE_SCROLL;
      matrix.setTextWrap(false);
    }
    else if (mode == MODE_SCROLL) {
      //Serial.println(F("edit mode: false -> true."));
      mode = MODE_EDIT;
      matrix.setTextWrap(true);
      loadScreen();
    }
  }
  modeWasPushed = modePushed;
}

void scrollSavedScreens() {
  if ((millis() - signalStart) > 10) {
    signalStart = millis();
    clearScreen();
    for (uint8_t s = FIRST_SCREEN; s <= LAST_SCREEN; s++) {
      matrix.setCursor(textX, ((s - FIRST_SCREEN) * 8));
      for (uint8_t i = 0 ; i < MAX_SCREEN; i++ ) {
        uint8_t si = (s * MAX_SCREEN) + i;
        char c = (char)EEPROM.read(si);
        matrix.print(c);
        //Serial.print(c);
      }
      //Serial.println(' ');
    }
    if ((--textX) < TEXT_MIN) {
      textX = matrix.width();
      setRandomTextColor();
    }
  }
}

void loadScreen() {
  clearScreen();
  int lastNonSpace = MAX_SCREEN;
  while (--lastNonSpace >= 0 ) {
    uint8_t si = (screen * MAX_SCREEN) + lastNonSpace;
    char c = (char)EEPROM.read(si);
    if (!isWhitespace(c)) {
      break;
    }
  }

  for (uint8_t i = 0 ; i <= lastNonSpace; i++ ) {
    setRandomTextColor();
    uint8_t si = (screen * MAX_SCREEN) + i;
    char c = (char)EEPROM.read(si);
    matrix.print(c);
    text[sPos++] = c;
    //Serial.print(c);
  }
}

void saveScreen() {
  for (uint8_t i = 0 ; i < MAX_SCREEN; i++ ) {
    uint8_t si = (screen * MAX_SCREEN) + i;
    char c = (char)EEPROM.read(si);
    if (c != text[i]) {
      EEPROM.write(si, text[i]);
    }
  }
}

void clearScreen() {
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setCursor(0, 0);
  sPos = 0;
  for (uint8_t i = 0; i < MAX_SCREEN; i++) {
    text[i] = ' ';
  }
  clearMorse();
}

void doBackSpace() {
  if (sPos > 0) {
    clearMorse();
    text[--sPos] = ' ';
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.setCursor(0, 0);
    for (uint8_t i = 0; i < sPos; i++) {
      setRandomTextColor();
      matrix.print(text[i]);
      //Serial.print(text[i]);
    }
    //Serial.println(' ');  Serial.println(sPos);
  }
}

void clearMorse() {
  for (uint8_t i = 0; i < MAX_MORSE; i++) {
    morse[i] = NIL;
  }
}

void checkMorse() {

  boolean dotPushed = (LOW == digitalRead(DOTB));
  boolean dashPushed = (LOW == digitalRead(DASHB));
  //  if (dotPushed) Serial.println(F(" .dot btn pushed. "));
  //  if (dashPushed) Serial.println(F(" -dash btn pushed- "));

  if ((dotPushed && dotWasPushed) || (dashPushed && dashWasPushed)) {
    //    Serial.println(F("button still pushed"));
  }
  else if ((!dotPushed && dotWasPushed) || (!dashPushed && dashWasPushed)) {
    //    Serial.println(F("button released"));
    if ((millis() - signalStart) > DELAY) {
      if (dotWasPushed) {
        //Serial.println("DOT");
        morse[mPos++] = DOT;
      }
      else if (dashWasPushed) {
        //Serial.println("DASH");
        morse[mPos++] = DASH;
      }
    }
    //Serial.print(F("Morse:")); Serial.println(morse);
    //Serial.print(F("mPos:")); Serial.println(mPos);
  }
  else if ((dotPushed && !dotWasPushed) || (dashPushed && !dashWasPushed))
  {
    //Serial.println(F("button pushed"));
    signalStart = millis();
  }
  else if ((!dotPushed && !dotWasPushed) && (!dashPushed && !dashWasPushed))
  {
    if (mPos == MAX_MORSE || (((millis() - signalStart) > PAUSE ) && (mPos > 0)))
    {
      readMorse(morse);
      mPos = 0;
      clearMorse();
    }
  }
  dotWasPushed = dotPushed;
  dashWasPushed = dashPushed;
}

void setRandomTextColor() {
  matrix.setTextColor(matrix.ColorHSV(random(0, MAX_HUE), 255, 255, true));
}

void appendChar(char c) {
  if (sPos < MAX_SCREEN) {
    //Serial.print(F("Append to screen: "));  Serial.println(c);
    setRandomTextColor();
    matrix.print(c);
    text[sPos++] = c;
  }
}

void readMorse(char str[])
{
  //Serial.print("Decoding: ");
  //Serial.println(str);
  if (str[0] == DOT && str[1] == DASH && str[2] == DOT && str[3] == DASH && str[4] == DOT && str[5] == DASH) {
    appendChar('.');
  }
  else if (str[0] == DASH && str[1] == DASH && str[2] == DOT && str[3] == DOT && str[4] == DASH && str[5] == DASH) {
    appendChar(',');
  }
  else if (str[0] == DOT && str[1] == DOT && str[2] == DASH && str[3] == DASH && str[4] == DOT && str[5] == DOT) {
    appendChar('?');
  }
  else if (str[0] == DASH && str[1] == DOT && str[2] == DASH && str[3] == DOT && str[4] == DASH && str[5] == DASH) {
    appendChar('!');
  }
  else if (str[0] == DOT && str[1] == DASH && str[2] == DASH && str[3] == DOT && str[4] == DASH && str[5] == DOT) {
    appendChar('@');
  }
  else if (str[0] == DOT && str[1] == DASH && str[2] == DOT && str[3] == DOT && str[4] == DOT) {
    appendChar('&');
  }
  else if (str[0] == DOT && str[1] == DASH && str[2] == NIL && str[3] == NIL && str[4] == NIL) {
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
  else if (str[0] == DASH && str[1] == DASH && str[2] == DASH && str[3] == DASH && str[4] == NIL) {
    doBackSpace();
  }
}
