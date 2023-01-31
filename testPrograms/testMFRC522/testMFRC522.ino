#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <SPI.h>

/*This code can be used to test an RFID RC522 module if you suspect any faults.
 * Setup:
 *  3.3V -> 3.3V
 *  RST -> PIN 9
 *  GND -> GND
 *  IRQ -> Unused
 *  MISO -> PIN 12
 *  MOSI -> PIN 11
 *  SCK -> PIN 13
 *  SDA -> PIN 10
 *  
 *  This code is the basic example code given to dump thew information of an RFID tag when it is presented to the RFID Scanner.
 *  This code only works for unencrypted cards. It will not work for encrypted cards.
 */

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
