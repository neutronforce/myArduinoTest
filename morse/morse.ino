// include the library code:
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int ledPin = 13;  
const int buttonPin = 6;    

int ledState = HIGH;         
int buttonState = LOW;             
int lastButtonState = LOW;  

int pause_value = 250;  // depending on your skill and how fast your fingers are you can change this value to make typing a message faster or slower
long signal_length = 0;
long pause = 0;

String morse = "";
String dash = "-";
String dot = "*";

boolean cheker = false;

String textOld = "";
String textTop = "";
String text = "";

long lastDebounceTime = 0;  
long debounceDelay = 50;    

void setup() {
  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);
  lcd.cursor();

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  lcd.setCursor(0, 1);
}

void loop() {
 
  buttonState = digitalRead(buttonPin);
  
  if (buttonState && lastButtonState)       
  {
    ++signal_length;  
    if (signal_length>2*pause_value)
      {
          digitalWrite(13, HIGH); 
      }     
    
  }
  else if(!buttonState && lastButtonState)          //this part of the code happens when the button is released and it send either * or - into the buffer
  {
     
     if (signal_length>50 && signal_length<2*pause_value )
     {
       morse =  morse + dot;
     } 
      else if (signal_length>2*pause_value)
      {
        morse = morse +  dash;
      }
    signal_length=0; 
    digitalWrite(13, LOW); 
  }
  else if(buttonState && !lastButtonState)          // this part happens when the button is pressed and its use to reset several values
  {
    pause=0;
    cheker = true;
  }
  else if (!buttonState && !lastButtonState)
  {  
    ++pause;
    if (( pause>3*pause_value ) && (cheker))
    { 
      printaj(morse);
      cheker = false;
      morse = "";
    }
  }
  lastButtonState=buttonState;
  delay(1);
}

void lcdprint(String c){

  if(text.length() == 16){
    lcd.clear();
    lcd.home();
    lcd.print(text);
    lcd.setCursor(0, 1);
    textOld = textTop;
    textTop = text;
    text = "";
  }
    lcd.print(c);
  text += c;
}


void printaj(String str)  
{                                
                                 
  if (str=="*-")
    lcdprint("A");
  else if (str=="-***")  
    lcdprint("B");
  else if (str=="-*-*")  
    lcdprint("C");
  else if (str=="-**")  
    lcdprint("D");
  else if (str=="*")  
    lcdprint("E");
  else if (str=="**-*")  
    lcdprint("F");
  else if (str=="--*")  
    lcdprint("G");
  else if (str=="****")  
    lcdprint("H");
  else if (str=="**")  
    lcdprint("I");
  else if (str=="*---")  
    lcdprint("J");
  else if (str=="-*-")  
    lcdprint("K");
  else if (str=="*-**")  
    lcdprint("L");
  else if (str=="--")  
    lcdprint("M");
  else if (str=="-*")  
    lcdprint("N");
  else if (str=="---")  
    lcdprint("O");
  else if (str=="*--*")  
    lcdprint("P");
  else if (str=="--*-")  
    lcdprint("Q");
  else if (str=="*-*")  
    lcdprint("R");
  else if (str=="***")  
    lcdprint("S");
  else if (str=="-")  
    lcdprint("T");
  else if (str=="**-")  
    lcdprint("U");
  else if (str=="***-")  
    lcdprint("V");
  else if (str=="*--")  
    lcdprint("W");
  else if (str=="-**-")  
    lcdprint("X");
  else if (str=="-*--")  
    lcdprint("Y");
  else if (str=="--**")  
    lcdprint("Z");

  else if (str=="*----")  
    lcdprint("1");
  else if (str=="**---")  
    lcdprint("2");
  else if (str=="***--")  
    lcdprint("3");
  else if (str=="****-")  
    lcdprint("4");
  else if (str=="*****")  
    lcdprint("5");
  else if (str=="-****")
    lcdprint("6");
  else if (str=="--***")  
    lcdprint("7");
  else if (str=="---**")  
    lcdprint("8");
  else if (str=="----*")  
    lcdprint("9");
  else if (str=="-----")  
    lcdprint("0");

  else if(str=="**--")
    lcdprint(" ");
  else if(str=="----") {
    if(text.length() == 0 && textTop.length() > 0){
      text = textTop;
      textTop = textOld;  
      textOld = "";
    } 
    if (text.length() > 0 ) {
      text = text.substring(0, text.length() - 1);
      lcd.clear();
      lcd.home();
      lcd.print(textTop);
      lcd.setCursor(0,1);
      lcd.print(text);
    }
  }

}