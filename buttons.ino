const unsigned long AUTO_REPEAT_DELAY = 500; // 1.5 secondi prima della ripetizione
const unsigned long AUTO_REPEAT_INTERVAL = 50; // ogni 100 ms una variazione
unsigned long lastAutoAdjust = 0; // tempo per la ripetizione automatica


void btnPlusUpdate(){

  btnPlus.read();
  if (btnPlus.wasPressed()) {
    if (inMenu){
    } else if (inSubMenu){
    } else{
    config.trip_partial += 0.01;
    config.trip_total += 0.01;
    displayValues[0] = config.trip_partial;
    displayValues[1] = config.trip_total; 
    }
  }

  unsigned long now = millis();

  if (btnPlus.isPressed() && btnPlus.pressedFor(AUTO_REPEAT_DELAY)) {
    if (now - lastAutoAdjust > AUTO_REPEAT_INTERVAL) {
      lastAutoAdjust = now;
      config.trip_partial += 0.01;
      config.trip_total += 0.01;
      displayValues[0] = config.trip_partial;
      displayValues[1] = config.trip_total; 
    }
  } 



  updateScreen();
}


void btnMinusUpdate(){

  btnMinus.read();
  if (btnMinus.wasPressed()) {
    if (inMenu){
    } else if (inSubMenu){
    } else{
    config.trip_partial = max(0.0, config.trip_partial - 0.01);
    config.trip_total = max(0.0, config.trip_total - 0.01);
    displayValues[0] = config.trip_partial;
    displayValues[1] = config.trip_total; 
    }
  }

  unsigned long now = millis();

  if (btnMinus.isPressed() && btnMinus.pressedFor(AUTO_REPEAT_DELAY)) {
    if (now - lastAutoAdjust > AUTO_REPEAT_INTERVAL) {
      lastAutoAdjust = now;
      config.trip_partial = max(0.0, config.trip_partial - 0.01);
      config.trip_total = max(0.0, config.trip_total - 0.01);
      displayValues[0] = config.trip_partial;
      displayValues[1] = config.trip_total; 
    }
  } 

  updateScreen();
}