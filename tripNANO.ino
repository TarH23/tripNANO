// Needed libraries
#include <HT1621.h>
#include <JC_Button.h>


// --- Pin defined ---
//  -- LCD --
#define PIN_CS 10
#define PIN_WR 11
#define PIN_DATA 12

//  -- Buttons --
#define BTN_MODE 3
#define BTN_PLUS 4
#define BTN_MINUS 5

//  -- Sensor --
#define PIN_SENSOR 6  // button for debug
#define pin_sensor 2  // Real value for interrupt

// --- LCD object ---
HT1621 lcd;

// --- Buttons ---
Button btnMode(BTN_MODE, true, true, 50);
Button btnPlus(BTN_PLUS, true, true, 50);
Button btnMinus(BTN_MINUS, true, true, 50);

Button sensor(PIN_SENSOR, true, true, 50);

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
    0, // Total
    0  // Speed
};

void setup(){
  Serial.begin(9600);

  // Start buttons
  sensor.begin();
  btnMode.begin();
  btnPlus.begin();
  btnMinus.begin();

  // Start interrupt for wheel sensor
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pulsesCount, FALLING);


  loadConfig();

  lcdStart();

  
}

void loop(){
  sensorUpdate();
  btnPlusUpdate();
  btnMinusUpdate();
}


void pulsesCount(){
  pulses++;
}


void sensorUpdate() {

  byte pulsesTmp = pulses;
  if(pulsesTmp > 0)
  {
    config.trip_partial += (config.circumference / 1000000.0) * pulsesTmp; // Count 1 every 10 meters
    config.trip_total += (config.circumference / 1000000.0) * pulsesTmp; // Count 1 every 10 meters

      
    pulses -= pulsesTmp;  

    distanceAlreadySaved = false;
    previousMillisDistance = millis(); // Renew time for auto-save
    
    Serial.println(F("SENSOR detected"));
    displayValues[0] = config.trip_partial;
    displayValues[1] = config.trip_total;
    Serial.print("trip_partial: ");
    Serial.println(config.trip_partial, 6);

    updateScreen();
  }
}



