//This Function runs when the User selects to create a clone
void createClone() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Present a Card"));
  lcd.setCursor(1, 1);
  lcd.print(F("Below to Clone"));

  bool cardPresent = false;

  //Variables needed for reading card
  byte buffer[18];
  byte size = sizeof(buffer);

  digitalWrite(CS_SD, LOW);                //Enable SD Card
  myFile = SD.open(fileName, FILE_WRITE);  //Open file

  if (myFile) {
    digitalWrite(CS_SD, HIGH);  //Disable SD so RFID can work

    //Repeat loop until a card is presented to the reader
    while (!cardPresent) {
      if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        cardPresent = true;
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print(F("Reading"));
        lcd.setCursor(8, 1);
        lcd.print(F("Card..."));

        //Dump Card info to Serial (Debugging)
        //rfid.PICC_DumpToSerial(&(rfid.uid));

        //Read Card Info
        for (byte block = 0; block < 64; block++) {
          rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid.uid));  //Authenticate card with factory default key
          status = rfid.MIFARE_Read(block, buffer, &size);                                   //Read block

          if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Read() failed: "));
            Serial.println(rfid.GetStatusCodeName(status));
          } else {
            digitalWrite(CS_SD, LOW);  //Enable SD Card
            //Print Buffer to file
            for (int n = 0; n < 16; n++) {
              if (buffer[n] >= 0 && buffer[n] <= 9) myFile.print(0);
              myFile.print(buffer[n], HEX);
            }
            delay(10);                  //Allows RFID reader to catch up so enough buffer space
            digitalWrite(CS_SD, HIGH);  //Disable SD Card
          }
        }
      }
    }

    //Inform User when card has been read
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print(F("Done!"));
    lcd.setCursor(10, 0);
    lcd.write(3);
    delay(1000);

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();  // Stops expected communication with card so new communications can occur (i.e. other functions)

    //Give Card a Name
    String cloneName = nameClone("Save Clone As:");

    //Write Clone Name to File
    digitalWrite(CS_SD, LOW);  //Enable SD Card
    myFile.print(',');
    myFile.print(cloneName);
    myFile.print('\n');
    myFile.close();             //CLOSE FILE IMPORTANT
    digitalWrite(CS_SD, HIGH);  //Disable SD Card

    //Inform User of saved Clone
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Clone saved as"));
    lcd.setCursor(3, 1);
    lcd.print(cloneName);
    delay(2000);
  } else {
    errorMessage();  //Inform User if SD card file failure
  }

  // Disable SD card chip select pin (Switch back to MFRC522) - Just to make sure
  digitalWrite(CS_SD, HIGH);
}
