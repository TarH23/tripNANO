const unsigned long AUTO_REPEAT_DELAY = 500; // 0.5 secondi prima della ripetizione
const unsigned long RESET_TRIP_DELAY = 1000; // 1.0 sec to reset trip
const unsigned long AUTO_REPEAT_INTERVAL = 50; // ogni 100 ms una variazione
unsigned long lastAutoAdjust = 0; // tempo per la ripetizione automatica

void readBtn() {
  btnPlus.read();
  btnMinus.read();
  btnMode.read();
}

void btnPlusUpdate(){

  if (btnPlus.wasPressed()) {
    Serial.println("Button Plus pressed");

    if (inMenu){
    } else if (inSubMenu){
    } else{
    config.trip_partial += 0.01;
    config.trip_total += 0.01;
    displayValues[0] = config.trip_partial;
    displayValues[2] = config.trip_total; 
    }
  }

  unsigned long now = millis();

  if (btnPlus.isPressed() && btnPlus.pressedFor(AUTO_REPEAT_DELAY)) {
    Serial.println("Button Plus hold");

    if (now - lastAutoAdjust > AUTO_REPEAT_INTERVAL) {
      lastAutoAdjust = now;
      config.trip_partial += 0.01;
      config.trip_total += 0.01;
      displayValues[0] = config.trip_partial;
      displayValues[2] = config.trip_total; 
    }
  } 
  updateScreen();
}


void btnMinusUpdate(){

  if (btnMinus.wasPressed()) {
    Serial.println("Button Minus pressed");

    if (inMenu){
    } else if (inSubMenu){
    } else{
    config.trip_partial = max(0.0, config.trip_partial - 0.01);
    config.trip_total = max(0.0, config.trip_total - 0.01);
    displayValues[0] = config.trip_partial;
    displayValues[2] = config.trip_total; 
    }
  }

  unsigned long now = millis();

  if (btnMinus.isPressed() && btnMinus.pressedFor(AUTO_REPEAT_DELAY) &&
      !btnMode.isPressed()
  ) {
    if (now - lastAutoAdjust > AUTO_REPEAT_INTERVAL) {
      Serial.println("Button Minus hold");

      lastAutoAdjust = now;
      config.trip_partial = max(0.0, config.trip_partial - 0.01);
      config.trip_total = max(0.0, config.trip_total - 0.01);
      displayValues[0] = config.trip_partial;
      displayValues[2] = config.trip_total; 
    }
  } 

  updateScreen();
}


void btnModeUpdate() {

  static bool longPressHandled = false;

  // --- Reset Trip if hold ---
  if (btnMode.isPressed() && btnMode.pressedFor(RESET_TRIP_DELAY)) {

    if (!longPressHandled && config.showInDisplay == 0) {
      Serial.println("Button Mode HOLD --> RESET TRIP");

      config.trip_partial = 0.00;
      displayValues[0] = config.trip_partial;

      longPressHandled = true;  
    }

    return; 
  }

  // 
  if (btnMode.wasReleased()) {

    if (longPressHandled) {
      longPressHandled = false;
      return;
    }

    // --- Change screen ---
    Serial.println("Button Mode SHORT press --> cambio schermata");

    config.showInDisplay += 1;

    if (config.showInDisplay > 2)
      config.showInDisplay = 0;

    updateScreen();
  }
}