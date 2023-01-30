#include <LiquidCrystal.h>
LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // rs, en, d4, d5, d6, d7

const byte leftPin = 10;
const byte selectPin = 11;
const byte rightPin = 12;
const byte backPin = 13;

bool powerOff = false;
String menuItems[4] = {"Create Clone", "Use Clone", "Edit Clones", "Power Off"};

void menuFunctions(int menu, byte left, byte right) {
  if(menu == 1) {
    //lcd.clear();
    //lcd.setCursor(0,0);
    //lcd.print("Create Clone");
  }
  if(menu == 2){
    //lcd.clear();
    //lcd.setCursor(0,0);
    //lcd.print("Use Clone");
  }
  if(menu == 3){
    //lcd.clear();
    //lcd.setCursor(0,0);
    //lcd.print("Edit Clones");
  }
  if(menu == 4){
    //exit;
  }
}

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

template< typename T, size_t NumberOfSize > 
size_t arraySize (T (&) [NumberOfSize]){ return NumberOfSize; }
int numberOfMenuItems = arraySize(menuItems) - 1;
int currentIndex = 0;
int previousIndex = 1;
byte changeOfState = 0;
unsigned long previousMillis = millis();

/***********************************************************************************/

void setup() {
  // put your setup code here, to run once:

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
  // put your main code here, to run repeatedly:
  //while(powerOff != true){
    lcd.setCursor(0,0);
    lcd.print("Menu Select:");
    //lcd.setCursor(0,1);

    //When right button is pressed, move to the next menu option
    if(digitalRead(rightPin) == LOW && changeOfState == 0){
      ++currentIndex;
      //Prevents index being higher than the array size
      if(currentIndex > numberOfMenuItems ){
        currentIndex = numberOfMenuItems;
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
    
    //Display the current menu item choice before selection
    if(currentIndex != previousIndex){ 
      lcd.clear();
      if(currentIndex != 0){
        lcd.setCursor(0,1);
        lcd.write(1);
      }
      if(currentIndex != numberOfMenuItems){
        lcd.setCursor(15,1);
        lcd.write(2); 
      }
      lcd.setCursor(2,1);
      lcd.print(menuItems[currentIndex]);
      menuFunctions(currentIndex + 1, 0, 0);
      previousIndex = currentIndex;
    }
    
    //Uses elapsed time to reset the button state. This means code isn't repeated from the user holding the button down.
    if(millis() - previousMillis >= 400){
      previousMillis = millis();
      changeOfState = 0;
    }
  //} 
}
