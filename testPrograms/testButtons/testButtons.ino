
/*This code can be used to test multiple buttons in case you suspect a fault.
 * NOTE: Make sure you edit the code to inculde the number of buttons you are testing.
 * Setup:
 *  Button Prong (doesn't matter which) -> and PIN Number
 *  Button Prong (doesn't matter which) -> GND
 * 
 *  All buttons can be connected to the same GND pin but seperate Digital Pins ofc.
 */

bool button1 = false, button2 = false, button3 = false, button4 = false;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //Configure pins as an input and enable the internal pull-up resistor,
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(10) == LOW) {
    Serial.println("Button 1 was pressed");
 
  }
 if (digitalRead(11) == LOW) {
    Serial.println("Button 2 was pressed");
  }

 if (digitalRead(12) == LOW) {
    Serial.println("Button 3 was pressed");
  }

 if (digitalRead(13) == LOW) {
    Serial.println("Button 4 was pressed");
  }
}
