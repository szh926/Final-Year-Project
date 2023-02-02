#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <avr/sleep.h>

#include "menuFunctions.h";

/***Pin and Variable Setup**********************************************************************/

LiquidCrystal lcd(A5, A4, A3, A2, A1, A0); // rs, en, d4, d5, d6, d7

const byte leftPin = 0;
const byte selectPin = 1;
const byte rightPin = 2;
const byte backPin = 3;

String menuItems[] = {"Create Clone", "Use Clone", "Create Copy", "Edit Clones", "Power Off"};
String clonedCards[] = {"Card 1", "Card 2", "Card 3"};

template< typename T, size_t NumberOfSize > 
size_t arraySize (T (&) [NumberOfSize]){ return NumberOfSize; }
int numberOfItems = arraySize(menuItems) - 1;
int currentIndex = 0;
int previousIndex = 1;
byte changeOfState = 0;
unsigned long previousMillis = millis();

/***Functions***********************************************************************************/

//Used to reset the time and button state
void timeReset(){
  previousMillis = millis();
  changeOfState = 0;
}

//Used to rename or delete selected clone
void editSelectedClone(int clone){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("editing clone");
  lcd.setCursor(0,1);
  lcd.print(clonedCards[clone]);
  delay(5000);
  
}

//Used to set up clone for using
void useSelectedClone(int clone){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("using clone");
  lcd.setCursor(0,1);
  lcd.print(clonedCards[clone]);
  delay(5000);
}

void copySelectedClone(int clone){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("copying clone");
  lcd.setCursor(0,1);
  lcd.print(clonedCards[clone]);
  delay(5000);
}

/***Special Characters**************************************************************************/
byte leftArrow[8] = {
  B00010,
  B00110,
  B01110,
  B11110,
  B01110,
  B00110,
  B00010,
  B00000
};

byte rightArrow[8] = {
  B01000,
  B01100,
  B01110,
  B01111,
  B01110,
  B01100,
  B01000,
  B00000
};

/***Main Code**********************************************************************************/

void setup() {
  // put your setup code here, to run once:
  sleep_disable();
  detachInterrupt(backPin);
  
  //Initialise button pins
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(selectPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(backPin, INPUT_PULLUP);

  //Initialise LCD screen dimensions
  lcd.begin(16,2);

  //Create custom characters
  lcd.createChar(1,leftArrow);
  lcd.createChar(2,rightArrow);

  //Power on screen on startup
  lcd.setCursor(2,0);
  lcd.print("RFID");
  lcd.setCursor(8,1);
  lcd.print("Cloner");
  delay(3000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly
  lcd.setCursor(0,0);
  lcd.print("Menu Select:");

  //When right button is pressed, move to the next menu option
  if(digitalRead(rightPin) == LOW && changeOfState == 0){
    ++currentIndex;
    //Prevents index being higher than the array size
    if(currentIndex > numberOfItems ){
      currentIndex = numberOfItems;
    }
    changeOfState = 1;
    previousMillis = millis();
  }
  //When left button is pressed, move to the previous menu option
  else if(digitalRead(leftPin) == LOW && changeOfState == 0){
    currentIndex--;
    //Prevents the Index being less than the array size
    if(currentIndex < 0){
      currentIndex = 0;
    }
    changeOfState = 1;
    previousMillis = millis();
  }

  //When the select button is pressed, select that option in the menu and continue the code
  if(digitalRead(selectPin) == LOW && changeOfState == 0){
    changeOfState = 1;
    previousMillis = millis();
    
    menuFunctions(currentIndex);
    
    numberOfItems = arraySize(menuItems) - 1;
    currentIndex = 0;
    previousIndex = 1;
  }
    
  //Display the current menu item choice before selection
  if(currentIndex != previousIndex){ 
    lcd.clear();
    if(currentIndex != 0){
      lcd.setCursor(0,1);
      lcd.write(1);
    }
    if(currentIndex != numberOfItems){
      lcd.setCursor(15,1);
      lcd.write(2); 
    }
    lcd.setCursor(2,1);
    lcd.print(menuItems[currentIndex]);
    previousIndex = currentIndex;
  }
    
  //Uses elapsed time to reset the button state. This means code isn't repeated from the user holding the button down.
  if(millis() - previousMillis >= 400) timeReset();
}
