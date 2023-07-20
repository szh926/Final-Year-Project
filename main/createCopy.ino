//This Function runs when the User selects to Copy a clone to a Card
void createCopy(String cloneName) {
  //Open file to get card
  myFile = SD.open(fileName, FILE_READ);

  if (myFile) {
    myFile.seek(0);
    bool found = false;
    int startOfData;

    //Find cloneName in File
    while (!found) {
      startOfData = myFile.position();
      myFile.find(',');
      String word = myFile.readStringUntil('\n');
      if (word == cloneName) {
        found = true;
      }
    }

    //Inform User to present card to copy to (THIS WILL OVERWRITE THE CARDS INFO)
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Present a Card"));
    lcd.setCursor(2, 1);
    lcd.print(F("to Overwrite"));

    bool cardPresent = false;
    char buffer[32];
    byte hexValues[16];  //For converting the char ascii to equivalent byte
    byte block;

    //convert char array to byte array function
    auto getNum = [](char c) {
      return c > '9' ? c - 'A' + 10 : c - '0';
    };

    digitalWrite(CS_SD, HIGH);  //Disable SD so RFID can work

    //Set UID first - makes sure PICC fucntions doesn't get confused
    while (!cardPresent) {
      if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        cardPresent = true;
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print(F("Writing"));
        lcd.setCursor(8, 1);
        lcd.print(F("Card..."));

        myFile.seek(startOfData + 128);  //Start at block 4

        //Write Clone to Card - Code Modified From: https://github.com/miguelbalboa/rfid/blob/master/examples/RFID-Cloner/RFID-Cloner.ino
        for (int i = 4; i < 63; i++) {
          if (i == 7 || i == 11 || i == 15 || i == 19 || i == 23 || i == 27 || i == 31 || i == 35 || i == 39 || i == 43 || i == 47 || i == 51 || i == 55 || i == 59) {  // Skip - Overwriting Trailer Sectors on MIFARE cards makes card permanently unusable
            //Read block from buffer - must be done regardless
            digitalWrite(CS_SD, LOW);  //Enable SD so file can be read
            myFile.read(buffer, 32);
            digitalWrite(CS_SD, HIGH);  //Disable SD so RFID can work
            i++;
          }
          block = i;

          //Read block from buffer - must be done regardless
          digitalWrite(CS_SD, LOW);  //Enable SD so file can be read
          myFile.read(buffer, 32);
          digitalWrite(CS_SD, HIGH);  //Disable SD so RFID can work

          //Convert Buffer - Code Modified From: https://stackoverflow.com/questions/37581925/how-to-convert-a-char-array-to-a-byte-array
          byte *ptr = hexValues;
          for (char *idx = buffer; *idx; ++idx, ++ptr) {  // loops until it hits null character
            *ptr = (getNum(*idx++) << 4) + getNum(*idx);
          }

          // Authenticate using key A
          Serial.println(F("Authenticating using key A..."));
          status = (MFRC522::StatusCode)rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(rfid.uid));
          if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(rfid.GetStatusCodeName(status));
            return;
          }

          // Authenticate using key B
          Serial.println(F("Authenticating again using key B..."));
          status = (MFRC522::StatusCode)rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(rfid.uid));
          if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(rfid.GetStatusCodeName(status));
            return;
          }

          status = (MFRC522::StatusCode)rfid.MIFARE_Write(block, hexValues, 16);
          if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(rfid.GetStatusCodeName(status));
          }
        }
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();  //Remember to call this function after communicating with an authenticated PICC - otherwise no new communications can start

        //Write to Sector 0 after to prevent corrution
        myFile.seek(startOfData);

        //Read Block 0
        digitalWrite(CS_SD, LOW);  //Enable SD so file can be read
        myFile.read(buffer, 32);
        digitalWrite(CS_SD, HIGH);  //Disable SD so RFID can work

        //Convert buffer
        byte *ptr = hexValues;
        for (char *idx = buffer; *idx; ++idx, ++ptr) {  // loops until it hits null character
          *ptr = (getNum(*idx++) << 4) + getNum(*idx);
        }

        rfid.MIFARE_OpenUidBackdoor(false);  //Allows us to write to block 0
        rfid.MIFARE_Write((byte)0, hexValues, (byte)16);
      }
    }
    myFile.close();  //CLOSE FILE IMPORTANT
  } else {
    errorMessage();  //Inform User of SD card file failure
    return;
  }
  //Inform User when Clone has finished being written
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Successfully");
  lcd.setCursor(0, 1);
  lcd.print(F("Written to Card!"));
  delay(2000);
}
