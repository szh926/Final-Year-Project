#include <LiquidCrystal.h>
#include <avr/sleep.h>

//Used when menu option is selected
void menuFunctions(int menu) {
  switch(menu){
    //Create a Clone
    case 0:
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Present a Card");
      lcd.setCursor(1,1);
      lcd.print("Below to Clone");
      while(digitalRead(backPin) != LOW){

      }
      break;
    //Power Off Arduino - technically putting it into sleep mode
    case 4: 
      sleep_enable();
      attachInterrupt(digitalPinToInterrupt(backPin), setup, LOW);
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_cpu();
      sleep_disable();
      detachInterrupt(digitalPinToInterrupt(backPin));      
      setup();
      break;
    //Use a Clone or Edit a Clone  
    default:
      currentIndex = 0;
      previousIndex = 1;
      numberOfItems = arraySize(clonedCards) - 1;
      
      while(digitalRead(backPin) != LOW){
        //Repeat of code in loop(), will look into efficency if time at end
        if(digitalRead(rightPin) == LOW && changeOfState == 0){
          ++currentIndex;
          if(currentIndex > numberOfItems ){
            currentIndex = numberOfItems;
          }
          changeOfState = 1;
          previousMillis = millis();
        }
        else if(digitalRead(leftPin) == LOW && changeOfState == 0){
          currentIndex--;
          if(currentIndex < 0){
          currentIndex = 0;
          }
          changeOfState = 1;
          previousMillis = millis();
        }
        if(digitalRead(selectPin) == LOW && changeOfState == 0){
          changeOfState = 1;
          previousMillis = millis();

          if(menu == 1) {
            useSelectedClone(currentIndex);
          }
          else if(menu == 2) {
            copySelectedClone(currentIndex);
          }
          else if(menu ==3) {
            editSelectedClone(currentIndex);
          }
          
          currentIndex = 0;
          previousIndex = 1;
          return;
        }
        if(currentIndex != previousIndex){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Select Clone:");
          if(currentIndex != 0){
            lcd.setCursor(0,1);
            lcd.write(1);
          }
          if(currentIndex != numberOfItems){
            lcd.setCursor(15,1);
            lcd.write(2); 
          }
          lcd.setCursor(2,1);
          lcd.print(clonedCards[currentIndex]);
          previousIndex = currentIndex;
       }
       if(millis() - previousMillis >= 400) timeReset();
      }
      break;
  }
}