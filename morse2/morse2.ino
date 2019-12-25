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

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

char morse[5];
uint8_t mPos = 0;
unsigned short signalLength = 0;
//short hue = 0;
bool dotWasPushed = false;
bool dashWasPushed = false;

void setup() {
 // Serial.begin(9600);
  matrix.begin();
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setCursor(0, 0);
  matrix.setTextSize(1);
  matrix.setTextWrap(true);

  pinMode(DOTB, INPUT_PULLUP);
  pinMode(DASHB, INPUT_PULLUP);
  pinMode(JOYB, INPUT_PULLUP);
}

void loop() {
  checkMorse();
  delay(1);
}

void checkMorse() {
  boolean dotPushed = (LOW == digitalRead(DOTB));
  boolean dashPushed = (LOW == digitalRead(DASHB));

  if ((dotPushed && dotWasPushed) || (dashPushed && dashWasPushed)) { //button still pushed
    if (signalLength < USHRT_MAX) {
      signalLength++;
    }
  }
  else if ((!dotPushed && dotWasPushed) || (!dashPushed && dashWasPushed)) { //button released
    if (signalLength > DELAY) {
      if (dotWasPushed) {
   //     Serial.println("DOT");
        morse[mPos++] = DOT;
      }
      else if (dashWasPushed) {
   //     Serial.println("DASH");
        morse[mPos++] = DASH;
      }
    }

  }
  else if ((dotPushed && !dotWasPushed) || (dashPushed && !dashWasPushed)) // button pushed
  {
    signalLength = 0;
  }
  else if ((!dotPushed && !dotWasPushed) && (!dashPushed && !dashWasPushed))  //button not pushed
  {
    if (signalLength < USHRT_MAX) {
      signalLength++;
    }
    if (mPos == 5 || (( signalLength > PAUSE ) && (mPos > 0)))
    {
      readMorse(morse);
      mPos = 0;
      for (int i = 0; i < 5; i++) morse[i] = ' ';
    }
  }
  //if(dotPushed) Serial.print(" .dot btn pushed. ");
  //if(dashPushed) Serial.print(" -dash btn pushed- ");
  dotWasPushed = dotPushed;
  dashWasPushed = dashPushed;
}

void appendChar(char c) {
 // matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
  matrix.print(c);
 // hue += 7; if (hue >= 1536) hue -= 1536;
}

void readMorse(char str[])
{
//  Serial.print("Decoding:"); Serial.println(str);
  if (str[0] == DOT) {
    if (str[1] == DASH) {
      appendChar('A');
    }
  }

  //  else if (str == "-***")
  //    appendChar("B");
  //  else if (str == "-*-*")
  //    appendChar("C");
  //  else if (str == "-**")
  //    appendChar("D");
  //  else if (str == "*")
  //    appendChar("E");
  //  else if (str == "**-*")
  //    appendChar("F");
  //  else if (str == "--*")
  //    appendChar("G");
  //  else if (str == "****")
  //    appendChar("H");
  //  else if (str == "**")
  //    appendChar("I");
  //  else if (str == "*---")
  //    appendChar("J");
  //  else if (str == "-*-")
  //    appendChar("K");
  //  else if (str == "*-**")
  //    appendChar("L");
  //  else if (str == "--")
  //    appendChar("M");
  //  else if (str == "-*")
  //    appendChar("N");
  //  else if (str == "---")
  //    appendChar("O");
  //  else if (str == "*--*")
  //    appendChar("P");
  //  else if (str == "--*-")
  //    appendChar("Q");
  //  else if (str == "*-*")
  //    appendChar("R");
  //  else if (str == "***")
  //    appendChar("S");
  //  else if (str == "-")
  //    appendChar("T");
  //  else if (str == "**-")
  //    appendChar("U");
  //  else if (str == "***-")
  //    appendChar("V");
  //  else if (str == "*--")
  //    appendChar("W");
  //  else if (str == "-**-")
  //    appendChar("X");
  //  else if (str == "-*--")
  //    appendChar("Y");
  //  else if (str == "--**")
  //    appendChar("Z");
  //
  //  else if (str == "*----")
  //    appendChar("1");
  //  else if (str == "**---")
  //    appendChar("2");
  //  else if (str == "***--")
  //    appendChar("3");
  //  else if (str == "****-")
  //    appendChar("4");
  //  else if (str == "*****")
  //    appendChar("5");
  //  else if (str == "-****")
  //    appendChar("6");
  //  else if (str == "--***")
  //    appendChar("7");
  //  else if (str == "---**")
  //    appendChar("8");
  //  else if (str == "----*")
  //    appendChar("9");
  //  else if (str == "-----")
  //    appendChar("0");
}
