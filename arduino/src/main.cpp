#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <OneWire.h>

#include "display.h"


#define ONE_WIRE_BUS 4
#define RELAY_PIN 6
#define ACTIVE_SWITCH_PIN 2
#define ACTIVE_LED_PIN 13

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
Display display(&lcd);



// Flag to control whether the system is active or passive
bool isActive;

bool isRelayOn;

float targetTemperature;


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
  display.displayStatusMessage("Relay: ON");
}


void switchRelayOff(void) {
  if (!isActive || !isRelayOn) {
    return;
  }

  isRelayOn = false;
  digitalWrite(RELAY_PIN, LOW);
  display.displayStatusMessage("Relay: OFF");
}


void setupRelay(void) {
  pinMode(RELAY_PIN, OUTPUT);
  isRelayOn = false;
}


void setActiveMode() {
  isActive = true;
  digitalWrite(ACTIVE_LED_PIN, HIGH);
  display.displayStatusMessage("System: ACTIVE");
}


void setPassiveMode() {
  switchRelayOff();
  isActive = false;
  digitalWrite(ACTIVE_LED_PIN, LOW);
  display.displayStatusMessage("System: PASSIVE");
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

  display.begin();

  sensors.begin();

  setupModeControl();
  setupRelay();

  targetTemperature = readTemperature();
}


void loop(void) {
  bool isSwitchPressed = digitalRead(ACTIVE_SWITCH_PIN) == HIGH;
  if (isSwitchPressed) {
    toggleActiveMode();
  }

  float temperature = readTemperature();

  if (temperature < targetTemperature) {
    switchRelayOn();
  } else {
    switchRelayOff();
  }

  display.displayTemperature(temperature, targetTemperature);
}


String command = "";


void executeCommand(void) {
  targetTemperature = command.toFloat();
}


void serialEvent() {
  int input;
  while (Serial.available()) {
    input = Serial.read();
    if (input == '\n') {
      executeCommand();
      command = "";
    } else {
      command += (char)input;
    }
  }
}
