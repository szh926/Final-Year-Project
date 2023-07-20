//This Function runs when the User selects to edit one of their stored clones
void editClone(String cloneName) {
  //Inform User of Options
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Select Option"));
  lcd.setCursor(0, 1);

  currentIndex = 0;
  previousIndex = 1;
  numberOfItems = arraySize(editItems) - 1;

  while (digitalRead(backPin) != LOW) {
    //When right button is pressed, move to the next clone option
    if (digitalRead(rightPin) == LOW && changeOfState == 0) {
      ++currentIndex;
      if (currentIndex > numberOfItems) {
        currentIndex = numberOfItems;
      }
      changeOfState = 1;
      previousMillis = millis();
    }
    //When left button is pressed, move to the previous clone option
    else if (digitalRead(leftPin) == LOW && changeOfState == 0) {
      currentIndex--;
      if (currentIndex < 0) {
        currentIndex = 0;
      }
      changeOfState = 1;
      previousMillis = millis();
    }
    //When the select button is pressed, select that clone to perform menu function selected previously
    if (digitalRead(selectPin) == LOW && changeOfState == 0) {
      changeOfState = 1;
      previousMillis = millis();

      if (currentIndex == 0) {
        editName(cloneName);
      } else if (currentIndex == 1) {
        deleteClone(cloneName);
      }

      currentIndex = 0;
      previousIndex = 1;
      return;
    }
    //Display the current clone to select
    if (currentIndex != previousIndex) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Select Option:"));
      if (currentIndex != 0 && editItems[currentIndex] != NULL) {
        lcd.setCursor(0, 1);
        lcd.write(1);
      }
      if (currentIndex != numberOfItems && editItems[currentIndex + 1] != NULL) {
        lcd.setCursor(15, 1);
        lcd.write(2);
      }
      lcd.setCursor(2, 1);
      lcd.print(editItems[currentIndex]);
      previousIndex = currentIndex;
    }
    if (millis() - previousMillis >= 400) timeReset();
  }
}

//This function edits the name of the clone
void editName(String oldName) {
  //Open File and find previous cloneName
  digitalWrite(CS_SD, LOW);               //Enable SD card
  myFile = SD.open(fileName, FILE_READ);  //Open file
  int startOfName;

  //Create Pointer to end of cloneName
  if (myFile) {
    myFile.seek(0);
    bool found = false;

    //Find cloneName in File
    while (!found) {
      myFile.find(',');
      startOfName = myFile.position();
      String word = myFile.readStringUntil('\n');
      if (word != oldName) {
        //Serial.println(word + "wrong word");
      } else {
        //Serial.println("found " + word);
        //Serial.println(myFile.position()); // For debugging
        found = true;
      }
    }
    myFile.close();

    //Now Open File with Write Permissions
    myFile = SD.open(fileName, O_RDWR);  //Open file

    if (myFile) {
      String newName = nameClone("Edit Clone to:");
      myFile.seek(startOfName);  //Change Position to start of word
      myFile.print(newName);
      myFile.close();

      //Inform user that name has ben changed
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Clone Edited to"));
      lcd.setCursor(3, 1);
      lcd.print(newName);
      delay(2000);
    } else {
      errorMessage();  //Inform User if file couldn't be written to
    }
  } else {
    errorMessage();  //Inform User if file couldn't be written to
  }
}

//This function deletes the clone from the SD card - this is done by moving all data except that to be deleted into a new file and deleting the old file
void deleteClone(String c) {
  //Inform User of Deletion
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(c);
  lcd.setCursor(0, 1);
  lcd.print("being Deleted...");
  delay(1000);

  File newFile;
  String newFileName;

  //make new file the opposite name to the old file
  if (fileName == "CLONES.csv") newFileName = "CLONES2.csv";
  else newFileName = "CLONES.csv";

  myFile = SD.open(fileName, FILE_READ);  //Open old file to get position of clone to be deleted

  if (myFile) {
    myFile.seek(0);
    bool found = false;
    int startOfData;
    int endOfData;
    int n;

    //Find cloneName in File
    while (!found) {
      startOfData = myFile.position();
      myFile.find(',');
      String word = myFile.readStringUntil('\n');
      if (word == c) {
        found = true;
        endOfData = myFile.position();
      }
    }

    //Open new file
    newFile = SD.open(newFileName, FILE_WRITE);

    if (newFile) {
      byte buffer[32];
      myFile.seek(0);  //Reset old file pointer

      while (myFile.position() + 32 < startOfData) {  //write data until buffer overlaps data for deletion
        n = myFile.readBytes(buffer, 32);
        newFile.write(buffer, n);  //n used incase buffer isn't full e.g. last part of file
      }

      n = myFile.readBytes(buffer, (startOfData - myFile.position()));  //Write remaining bytes before position
      newFile.write(buffer, n);
      myFile.seek(endOfData);  //Skip data to be deleted

      while (myFile.available() > 0) {  //Continue writing rest of file
        n = myFile.readBytes(buffer, 32);
        newFile.write(buffer, n);
      }

      newFile.close();  //Close new file
    } else {
      errorMessage();  //Inform user if new file couldn't be opened
    }

    myFile.close();  //Close old file

    SD.remove(fileName);     //Delete old file
    fileName = newFileName;  //Rename current fileName to be the newFile

    //Inform User of Successful Deletion
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print(c);
    lcd.setCursor(3, 1);
    lcd.print("Deleted! ");
    lcd.write(3);
    delay(2000);
  } else {
    errorMessage();  //Inform user if new file couldn't be opened
  }
}