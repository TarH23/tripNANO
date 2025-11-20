// Needed libraries
#include <HT1621.h>
#include <JC_Button.h>


// --- Pin defined ---
//  -- LCD --
#define PIN_CS 10
#define PIN_WR 11
#define PIN_DATA 12

//  -- Case Buttons --
#define BTN_UP 8
#define BTN_DOWN 9

//  -- Handlebar Buttons --
#define BTN_MODE 3
#define BTN_PLUS 4
#define BTN_MINUS 5

//  -- Sensor --
// #define PIN_SENSOR 6  // button for debug
#define pin_sensor 2  // Real value for interrupt

// Variables for Speed calc
volatile unsigned long lastSpeedPulseTime = 0;
unsigned long lastPulseMillis = 0;
unsigned long pulseInterval = 0;
unsigned long lastDisplaySpeedUpdate = 0;
const unsigned long DISPLAY_SPEED_INTERVAL = 1000;
const unsigned long SPEED_DISPLAY_INTERVAL = 1000; // 1 secondo
unsigned long lastSpeedDisplayUpdate = 0;


float filteredSpeed = 0;
const float SPEED_FILTER = 0.20;  // più alto = più reattivo

float speed = 0.0;                      // speed in km/h

// --- LCD object ---
HT1621 lcd;

// --- Buttons ---
Button btnUp(BTN_UP, true, true, 50);
Button btnDown(BTN_DOWN, true, true, 50);
Button btnMode(BTN_MODE, true, true, 50);
Button btnPlus(BTN_PLUS, true, true, 50);
Button btnMinus(BTN_MINUS, true, true, 50);

// Button sensor(PIN_SENSOR, true, true, 50);

// Interruption variables for wheel sensor
const byte interruptPin = pin_sensor;
volatile byte pulses = 0;


struct Configuration {
  int version;
  int showInDisplay;
  int circumference;
  float trip_partial = 0;
  float trip_total = 0;
} config;

// Menu variables
int selectedMenuOption = 0;
int selectedSubMenuOption = 0;

bool inMenu = false;
bool inSubMenu = false;


unsigned long previousMillis = 0;
unsigned long previousMillisSpeed = 0;
unsigned long previousMillisDistance = 0;
const long displayRefreshInterval = 500;
const long autoSaveAfter = 3000;
bool distanceAlreadySaved = true;

float displayValues[] = {
    0, // Partial
    0, // Speed
    0  // Total
};

void setup(){
  Serial.begin(9600);

  // Start buttons
  // sensor.begin();

  btnPlus.begin();
  btnMinus.begin();
  btnMode.begin();

  // Start interrupt for wheel sensor
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pulsesCount, FALLING);


  loadConfig();

  lcdStart();

  
}

void loop(){
  sensorUpdate();
  
  speedUpdate();

  
  readBtn();
  btnPlusUpdate();
  btnMinusUpdate();
  btnModeUpdate();
}


void pulsesCount(){
  pulses++;
  //pulses += 100;
}


void sensorUpdate() {

  byte pulsesTmp = pulses;
  if(pulsesTmp > 0){

    unsigned long now = millis();
    pulseInterval = now - lastPulseMillis;   // tempo ultimo impulso
    lastPulseMillis = now;

    float meters = (config.circumference / 1000.0) * pulsesTmp;

    config.trip_partial += meters / 1000.0;
    config.trip_total   += meters / 1000.0;
      
    pulses -= pulsesTmp;  

    distanceAlreadySaved = false;
    previousMillisDistance = millis(); // Renew time for auto-save
    
    Serial.println(F("SENSOR detected"));
    displayValues[0] = config.trip_partial;
    displayValues[2] = config.trip_total;
    Serial.print("trip_partial: ");
    Serial.println(config.trip_partial, 6);

    updateScreen();
  }
}

void speedUpdate(){

  speed = calculateSpeed();


  if (millis() - lastSpeedDisplayUpdate >= SPEED_DISPLAY_INTERVAL) {
      lastSpeedDisplayUpdate = millis();

      displayValues[1] = speed;   // velocità calcolata continuamente
      updateScreen();             // mostra la velocità 1 volta al secondo
  }

}



float calculateSpeed() {

  if (pulseInterval == 0 || (millis() - lastPulseMillis) > 500) {
    filteredSpeed = 0;
    return 0;
  }

  // mm → metri
  float metersPerPulse = config.circumference / 1000.0;

  // tempo tra impulsi in secondi
  float seconds = pulseInterval / 1000.0;

  // m/s → km/h
  float speed_inst = (metersPerPulse / seconds) * 3.6;

  // filtro per evitare oscillazioni
  filteredSpeed = filteredSpeed + SPEED_FILTER * (speed_inst - filteredSpeed);

  return filteredSpeed;
}




