#include <MFRC522.h>        //RFID Scanner
#include <SPI.h>            //Serial Input
#include <SD.h>             //SD Card Module
#include <LiquidCrystal.h>  //LCD Screen
#include <avr/sleep.h>      //Sleep Mode

#include "menuFunctions.h";
#include "createClone.h";
#include "createCopy.h";
#include "editClone.h";

/***** Pin Setup *************************************************************************************/

//Defined Pins for the MFRC522 Module
#define RST_RFID 9
#define CS_RFID 10

//Defined Chip Select Pin for the SD Card Module
#define CS_SD 8

//Defined Pins for the buttons to control screen
constexpr byte leftPin = 0;
constexpr byte selectPin = 1;
constexpr byte rightPin = 2;
constexpr byte backPin = 3;

//Initialise LCD Screen pins
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);  // rs, en, d4, d5, d6, d7

//Initialise MFRC522 pins
MFRC522 rfid(CS_RFID, RST_RFID);  // Create MFRC522 instance

/***** Variable Setup *******************************************************************************/

char letters[27] = { ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
String menuItems[] = { "Create Clone", "Create Copy", "Edit Clones", "Power Off" };
String editItems[2] = { "Edit Name", "Delete Clone" };
String clonedCards[6];  //Changeable depending on Capacity of SD Card and Arduino

//For controlling menus - Code Modified From: https://myhomethings.eu/en/dynamic-arduino-lcd-menu/
template< typename T, size_t NumberOfSize >
size_t arraySize(T (&)[NumberOfSize]) {
  return NumberOfSize;
}
int numberOfItems = arraySize(menuItems) - 1;
int currentIndex = 0;
int previousIndex = 1;
byte changeOfState = 0;
unsigned long previousMillis = millis();

//File to Store Cloned Cards
File myFile;
//File Name (changes when variable is deleted)
String fileName;

//Create Key Struct
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;  // status codes (used in debugging)

/***** Global Functions ******************************************************************************/

//This function is used to reset the time and button state, allowing button functionality
void timeReset() {
  previousMillis = millis();
  changeOfState = 0;
}

//This function is used to inform the user when a file couldn't be opened
void errorMessage() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("Error with"));
  lcd.setCursor(5, 1);
  lcd.print(F("File "));
  lcd.write(4);
  delay(3000);
}

//This Function allows the user to name a clone using the buttons on the LCD screen
String nameClone(String message) {
  currentIndex = 0;
  previousIndex = 1;
  int numberOfLetters = arraySize(letters) - 1;
  String name;  //Clone Name
  int pos = 3;  //LCD cursor position

  //Inform User to name Clone
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);

  //Set Clone Name - Similar to menu code in loop()
  while (pos < 13) {
    lcd.setCursor(pos, 1);
    lcd.cursor();

    //When right button is pressed, move to the display next letter in alphabet
    if (digitalRead(rightPin) == LOW && changeOfState == 0) {
      ++currentIndex;
      //Prevents index being higher than the array size
      if (currentIndex > numberOfLetters) {
        currentIndex = numberOfLetters;
      }
      changeOfState = 1;
      previousMillis = millis();
    }
    //When left button is pressed, move to the previous letter in alphabet
    else if (digitalRead(leftPin) == LOW && changeOfState == 0) {
      currentIndex--;
      //Prevents the Index being less than the array size
      if (currentIndex < 0) {
        currentIndex = 0;
      }
      changeOfState = 1;
      previousMillis = millis();
    }

    //When the select button is pressed, select that letter and continue
    if (digitalRead(selectPin) == LOW && changeOfState == 0) {
      changeOfState = 1;
      previousMillis = millis();

      if (currentIndex == 0) {
        name += " ";
      } else {
        name += letters[currentIndex];
        lcd.print(letters[currentIndex]);
      }

      pos += 1;

      currentIndex = 0;
      previousIndex = 1;
    }

    //Display the current menu item choice before selection
    if (currentIndex != previousIndex) {
      lcd.print(' ');
      if (currentIndex != 0) {
        lcd.setCursor(0, 1);
        lcd.write(1);
      }
      if (currentIndex != numberOfLetters) {
        lcd.setCursor(15, 1);
        lcd.write(2);
      }
      lcd.setCursor(pos, 1);
      lcd.print(letters[currentIndex]);
      previousIndex = currentIndex;
    }
    if (millis() - previousMillis >= 400) timeReset();
  }
  lcd.noCursor();
  return name;
}

/***** Special Characters **************************************************************************/
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

byte smile[8] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};

byte sad[8]{
  B00000,
  B00000,
  B01010,
  B00000,
  B01110,
  B10001,
  B00000,
  B00000
};

/***** Main Code **********************************************************************************/

void setup() {
  // put your setup code here, to run once:
  //Sleep Interrupt
  sleep_disable();
  detachInterrupt(backPin);

  //Initialise button pins
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(selectPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(backPin, INPUT_PULLUP);

  //Initialise LCD screen brightness and dimensions
  analogWrite(5, 75);
  lcd.begin(16, 2);

  //Create custom characters
  lcd.createChar(1, leftArrow);
  lcd.createChar(2, rightArrow);
  lcd.createChar(3, smile);
  lcd.createChar(4, sad);

  //Initialise Serial Communication between UNO, MFRC522 and SD Card Module
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  //Sets key (Authentication code) to factory default

  //Initialise SD Card
  if (!SD.begin(CS_SD)) {
    lcd.setCursor(0, 0);
    lcd.print(F("SD card Failure!"));  //Lets user know the SD card can't be read
    delay(3000);
    return;
  }

  if (SD.exists("CLONES.csv")) fileName = "CLONES.csv";  //Two possible file names needed for deleting clones
  if (SD.exists("CLONES2.csv")) fileName = "CLONES2.csv";

  //Start Screen
  lcd.clear();
  char* messageRFID = "  RFID    ";
  char* messageCloner = "        Cloner  ";

  //Scrolling display alternating on each row. Code Modified From: https://www.instructables.com/Scroll-a-Single-LCD-Line-In-How-to/
  for (int j = 40 - strlen(messageRFID); j <= 56 - strlen(messageRFID); j++) {
    for (int k = 0; k <= 15; k++) {
      lcd.print(" ");  // Clear line
    }
    lcd.setCursor(j, -1);
    lcd.print(messageRFID);
    lcd.setCursor(40 - j, 1);
    for (int n = 0; n <= j - 24; n++) {
      lcd.print(messageCloner[n]);
    }
    delay(250);
  }
  delay(250);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly
  //When right button is pressed, move to the next menu option
  if (digitalRead(rightPin) == LOW && changeOfState == 0) {
    ++currentIndex;
    //Prevents index being higher than the array size
    if (currentIndex > numberOfItems) {
      currentIndex = numberOfItems;
    }
    changeOfState = 1;
    previousMillis = millis();
  }
  //When left button is pressed, move to the previous menu option
  else if (digitalRead(leftPin) == LOW && changeOfState == 0) {
    currentIndex--;
    //Prevents the Index being less than the array size
    if (currentIndex < 0) {
      currentIndex = 0;
    }
    changeOfState = 1;
    previousMillis = millis();
  }

  //When the select button is pressed, select that option in the menu and continue the code
  if (digitalRead(selectPin) == LOW && changeOfState == 0) {
    changeOfState = 1;
    previousMillis = millis();

    menuFunctions(currentIndex);

    numberOfItems = arraySize(menuItems) - 1;
    currentIndex = 0;
    previousIndex = 1;
  }

  //Display the current menu item choice before selection
  if (currentIndex != previousIndex) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Menu Select:"));
    if (currentIndex != 0) {
      lcd.setCursor(0, 1);
      lcd.write(1);
    }
    if (currentIndex != numberOfItems) {
      lcd.setCursor(15, 1);
      lcd.write(2);
    }
    lcd.setCursor(2, 1);
    lcd.print(menuItems[currentIndex]);
    previousIndex = currentIndex;
  }

  //Uses elapsed time to reset the button state. This means code isn't repeated from the user holding the button down.
  if (millis() - previousMillis >= 400) timeReset();
}
