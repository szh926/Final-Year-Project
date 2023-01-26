#include <LiquidCrystal.h>

LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // rs, en, d4, d5, d6, d7

/*Test LCD Screen if you suspect a fault.
 * Setup: (Referance: https://arduinogetstarted.com/tutorials/arduino-lcd)
 *  VSS -> GND
 *  VDD -> 5V
 *  V0 -> Potentiometer middle pin
 *  RS -> PIN 1
 *  RW -> GND
 *  E -> PIN 2
 *  D4 -> PIN 4
 *  D5 -> PIN 5
 *  D6 -> PIN 6
 *  D7 -> PIN 7
 *  A -> 5V
 *  K -> GND
 *  
 *  Potentiometer left pin -> 5V
 *  Potentiometer right pin -> GND
 *  
 *  
 *This code can be used to test the functionality of your LCD screen but also as a reminder of certain LCD screen functions.
 */

byte smile[8] = { //can create own characters using byte
  B00000,
  B01110,
  B10101,
  B11111,
  B01110,
  B10001,
  B01110,
  B00000
};

byte lock[8] = {
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};

char array1[] = "Hello, World!               "; // can set strings as arrays to print on LCD

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2); //initialise LCD screen dimensions
  lcd.createChar(0,smile); 
  lcd.createChar(1,lock);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,0);
  lcd.print("AaBbCcDdEeFfGgHh");
  lcd.setCursor(0,2);
  lcd.print("IiJjKkLlMmNnOoPp");
  delay(5000);

  lcd.clear();
  lcd.setCursor(15,0);
  
  for ( int positionCounter1 = 0; positionCounter1 < 28; positionCounter1++) {
    lcd.scrollDisplayLeft();  //Scrolls the contents of the display one space to the left
    lcd.print(array1[positionCounter1]);  // Print a message to the LCD
    delay(500);
  }

  lcd.clear();

  lcd.setCursor(5,0); //Show Cursor Position
  lcd.blink();
  delay(3000);
  lcd.noBlink();
  
  lcd.setCursor(5,1); //Print special Characters
  lcd.write(byte(0));
  delay(3000);

  lcd.setCursor(9,1);
  lcd.write(1);
  delay(3000);

  lcd.clear();

  
}
