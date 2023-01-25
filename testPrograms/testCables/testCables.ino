#define LED_PIN 12

/*Test jumper wires and dupoint wires using this code.
 *Setup:
 *  One cable connected to defined pin above and longer end of LED
 *  Second cable connected to shorter end of LED and GND
 *  
 *Switch out cables as needed to test functionality.
 */


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
