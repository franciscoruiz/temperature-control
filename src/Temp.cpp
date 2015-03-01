#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
#define RELAY_PIN 10
#define ACTIVE_SWITCH_PIN 2
#define ACTIVE_LED_PIN 13

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Flag to control whether the system is active or passive (read-only)
bool isActive;

bool isRelayOn;


float readTemperature(void) {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}


void switchRelayOn(void) {
  if (!isActive || isRelayOn) {
    return;
  }

  isRelayOn = true;
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Switching relay on");
}


void switchRelayOff(void) {
  if (!isActive || !isRelayOn) {
    return;
  }

  isRelayOn = false;
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("Switching relay off");
}


void setupRelay(void) {
  pinMode(RELAY_PIN, OUTPUT);
  isRelayOn = false;
}


void setActiveMode() {
  isActive = true;
  digitalWrite(ACTIVE_LED_PIN, HIGH);
  Serial.println("System is now active");
}


void setPassiveMode() {
  switchRelayOff();
  isActive = false;
  digitalWrite(ACTIVE_LED_PIN, LOW);
  Serial.println("System is now passive");
}


void toggleActiveMode() {
  if (isActive) {
    setPassiveMode();
  } else {
    setActiveMode();
  }
}


void setupModeControl(void) {
  isActive = false;
  pinMode(ACTIVE_LED_PIN, OUTPUT);
}


void setup(void) {
  Serial.begin(9600);
  Serial.println("Starting up Temperature Monitor/Controller");

  sensors.begin();
  
  setupModeControl();
  setupRelay();
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

