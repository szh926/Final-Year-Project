//This Function runs when a menu option is selected
void menuFunctions(int menu) {
  switch (menu) {
    //Create a Clone
    case 0:
      while (digitalRead(backPin) != LOW) {
        createClone();
        break;
      }
      return;
    //Power Off Arduino - work on if time at end
    case 3:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(F("Powering"));
      lcd.setCursor(9, 1);
      lcd.print(F("Off..."));
      delay(3000);

      sleep_enable();
      attachInterrupt(digitalPinToInterrupt(backPin), setup, LOW);
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_cpu();
      sleep_disable();
      detachInterrupt(digitalPinToInterrupt(backPin));
      setup();
      break;
    //Create Copy or Edit a Clone
    default:
      //Update array to have current clone names - takes time so put loading screen
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print(F("Loading ..."));

      if (!getCloneNames()) return;

      lcd.clear();

      currentIndex = 0;
      previousIndex = 1;
      numberOfItems = arraySize(clonedCards) - 1;

      //Give user Clone choices, Similar to main code in loop() - will look into efficency if time at end
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

          if (menu == 1) {
            createCopy(clonedCards[currentIndex]);
            break;
          } else if (menu == 2) {
            editClone(clonedCards[currentIndex]);
            break;
          }

          currentIndex = 0;
          previousIndex = 1;
          return;
        }
        //Display the current clone to select
        if (currentIndex != previousIndex) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("Select Clone:"));
          if (currentIndex != 0 && clonedCards[currentIndex] != NULL) {
            lcd.setCursor(0, 1);
            lcd.write(1);
          }
          if (currentIndex != numberOfItems && clonedCards[currentIndex + 1] != NULL) {
            lcd.setCursor(15, 1);
            lcd.write(2);
          }
          lcd.setCursor(2, 1);
          lcd.print(clonedCards[currentIndex]);
          previousIndex = currentIndex;
        }
        if (millis() - previousMillis >= 400) timeReset();
      }
      break;
  }
}


//This Function collects the names of all the clones saved to the SD card and stores them in an array
bool getCloneNames() {
  myFile = SD.open(fileName, FILE_READ);
  myFile.seek(0);

  if (myFile) {  //If File opened successfully
    //Serial.println("File Opened!!");
    for (int i = 0; i < arraySize(clonedCards); i++) {
      myFile.find(',');
      String text = myFile.readStringUntil('\n');
      clonedCards[i] = text;
    }
    myFile.close();
  } else {
    errorMessage();
    return false;
  }
  return true;
  ;
}