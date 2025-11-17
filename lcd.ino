// Menus

char* menu[] = {
  "Circ\0",
  "Reset"
};



void updateScreen() {
  if(inMenu){
    lcd.print(menu[selectedMenuOption]);
    lcd.clear();
  } else if(inSubMenu){
    lcd.print(menu[selectedMenuOption]);

    switch(selectedMenuOption){
      case 0:
      // Circonference
      lcd.print(config.circumference, 0);
      break;

      case 1:
      // Reset to default
      lcd.print(config.circumference, 0);
      break;

    }
  } else {
    switch(config.showInDisplay){
      case 0:
       // Partial
        lcd.print(displayValues[0], 2);
        break;
      case 1:
       // Total
        lcd.print(displayValues[1], 2);
        break;
      case 2:
       // Speed km/h
        lcd.print(displayValues[2], 2);
        break;
    }
  }
}


void lcdStart() {
  // Start lcd
  lcd.begin(PIN_CS, PIN_WR, PIN_DATA);
  lcd.clear();
  // lcd.print("READY");
  // delay(1000);
  // lcd.print(config.circumference,0);
  // delay(2000);
  // lcd.clear();
  updateScreen();
}