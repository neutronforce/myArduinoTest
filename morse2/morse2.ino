#include <RGBmatrixPanel.h>

#define CLK  8  
#define OE   9
#define LAT  10
#define DOT  11
#define DASH 12
#define MODE 13
#define A    A0
#define B    A1
#define C    A2
#define D    A3
#define Y    A4
#define X    A5

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

int textX[4] = {matrix.width(), matrix.width(), matrix.width(), matrix.width()};

void setup() {

  matrix.begin();
  fillBlack();

  matrix.setCursor(0, 0); 
  matrix.setTextSize(1);    
  matrix.setTextWrap(false);

  pinMode(DOT, INPUT_PULLUP);  
  pinMode(DASH, INPUT_PULLUP);
  pinMode(MODE, INPUT);
}

void loop() {

}

void fillBlack(){
    matrix.fillScreen(matrix.Color333(0, 0, 0));
}

void setWhite(){
    matrix.setTextColor(matrix.Color333(7,7,7));
}

void setYellow(){
    matrix.setTextColor(matrix.Color333(7,7,0));
}

void setOrange(){
    matrix.setTextColor(matrix.Color333(7,4,0));
}

void setPink(){
    matrix.setTextColor(matrix.Color333(7,0,4));
}

void setAqua(){
   matrix.setTextColor(matrix.Color333(0,7,7));
}

void setBlue(){
   matrix.setTextColor(matrix.Color333(0,0,7));
}

void setNavy(){
   matrix.setTextColor(matrix.Color333(0,0,4));
}

void setMagenta(){
    matrix.setTextColor(matrix.Color333(7,0,7));
}

void setPurple(){
    matrix.setTextColor(matrix.Color333(4,0,4));
}

void setRed(){
    matrix.setTextColor(matrix.Color333(7,0,0));
}

void setGreen(){
    matrix.setTextColor(matrix.Color333(0,7,0));
}
