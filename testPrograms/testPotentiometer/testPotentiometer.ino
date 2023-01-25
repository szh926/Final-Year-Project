/*Test Poteniometer if you suspect a fault.
 * Setup:
 *  5V cable connected to right pin of potentiometer. 
 *  GND cable connected to left pin of potentiometer. 
 *  A0 cable connected to middle pin of potentiometer to read result.
 *  
 *Arduino converts the Analog voltage into a digital (ADC) so we can how the volts differ using the potentiometer. 
 *Shaft turned completely one direction should make value ) and other idrection should make value 1023.
 */


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(1);  // delay in between reads for stability
}
