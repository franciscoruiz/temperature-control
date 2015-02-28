#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
#define RELAY_PIN 10
#define ACTIVE_SWITCH_PIN 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Flag to control whether the system is active or passive (read-only)
bool isActive = false;


float readTemperature(void) {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}


void switchRelayOn(void) {
  if (!isActive) {
    return;
  }

  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Switching relay on");
}


void switchRelayOff(void) {
  if (!isActive) {
    return;
  }
  
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("Switching relay off");
}


void setActiveMode() {
  Serial.println("System is now active");
  isActive = true;
}


void setPassiveMode() {
  switchRelayOff();
  Serial.println("System is now passive");
  isActive = false;
}


void toggleActiveMode() {
  if (isActive) {
    setPassiveMode();
  } else {
    setActiveMode();
  }
}


void setup(void) {
  Serial.begin(9600);
  Serial.println("Starting up Temperature Monitor/Controller");

  sensors.begin();
  
  pinMode(RELAY_PIN, OUTPUT);
  switchRelayOff();
}


void loop(void) { 
  bool isSwitchPressed = digitalRead(ACTIVE_SWITCH_PIN) == HIGH;
  if (isSwitchPressed) {
    toggleActiveMode();
  }
  
  float temperature = readTemperature();
  Serial.println(temperature);
  
  if (temperature < 28.0) {
    switchRelayOn();
  } else {
    switchRelayOff();
  }
}

