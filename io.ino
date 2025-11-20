#include <EEPROM.h>
#include <Arduino.h> 

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
  const byte* p = (const byte*)(const void*)&value;
  byte c;  // Comparison byte
  int i;
  for (i = 0; i < sizeof(value); i++) {
    c = EEPROM.read(ee);
    if(c != *p) {
      EEPROM.write(ee, *p);
    }
    *p++;
    ee++;
  }
  return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
  byte* p = (byte*)(void*)&value;
  int i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}


void loadConfig() {
  Serial.println(F("---> Loading saved data..."));

  EEPROM_readAnything(0, config);

  Serial.println(config.version);
  Serial.println(config.showInDisplay);
  Serial.println(config.circumference);
  Serial.println(config.trip_partial);
  Serial.println(config.trip_total);

  if(config.version != 123){
    Serial.println(F("---> No configuration data found. Setting up default values..."));
    
    config.showInDisplay = 0; // Partial distance
    config.circumference = 2100; // KTM 125 exc 2008
    config.version = 123;
    config.trip_partial = 0;
    config.trip_total = 0;

    saveConfig();
  } else {
    displayValues[0] = config.trip_partial;
    displayValues[1] = config.trip_total;
  }

  Serial.println(F("---> DONE"));
}

void saveConfig(){
  EEPROM_writeAnything(0, config);
  Serial.println(F("---> CONFIG SAVED!"));

  Serial.println(config.version);
  Serial.println(config.showInDisplay);
  Serial.println(config.circumference);
  Serial.println(config.trip_partial);
  Serial.println(config.trip_total);
}
