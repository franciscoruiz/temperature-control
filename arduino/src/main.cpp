#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <OneWire.h>


#define ONE_WIRE_BUS 4
#define RELAY_PIN 10
#define ACTIVE_SWITCH_PIN 2
#define ACTIVE_LED_PIN 13

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define STATUS_MESSAGE_MILLIS 2000

unsigned long lcd_timer;

// Flag to control whether the system is active or passive
bool isActive;

bool isRelayOn;

int targetTemperature;


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
  lcdPrint("Switching relay on");
}


void switchRelayOff(void) {
  if (!isActive || !isRelayOn) {
    return;
  }

  isRelayOn = false;
  digitalWrite(RELAY_PIN, LOW);
  lcdPrint("Switching relay off");
}


void setupRelay(void) {
  pinMode(RELAY_PIN, OUTPUT);
  isRelayOn = false;
}


void setActiveMode() {
  isActive = true;
  digitalWrite(ACTIVE_LED_PIN, HIGH);
  lcdPrint("System is now active");
}


void setPassiveMode() {
  switchRelayOff();
  isActive = false;
  digitalWrite(ACTIVE_LED_PIN, LOW);
  lcdPrint("System is now passive");
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


bool isDisplayLocked = false;

bool lockDisplay () {
  isDisplayLocked = true;
  lcd_timer = millis();
}

bool releaseDisplay() {
  isDisplayLocked = false;
}


bool isDisplayAvailable() {
  if (!isDisplayLocked) {
    return true;
  }

  bool isTimerExpired = millis() - lcd_timer > STATUS_MESSAGE_MILLIS;
  if (isTimerExpired) {
    releaseDisplay();
  }
  return isTimerExpired;
}


void lcdPrint(char[] line1) {
  lcdPrint(line1, "");
}


void lcdPrint(char[] line1, char[] line2) {
  lcd.setCursor(0, 0);
  lcd.print(line1);

  lcd.setCursor(0, 1);
  lcd.print(line2);
}


void displayStatusMessage(char[] message) {
  lockDisplay();
  lcdPrint(message);
}


void displayTemperature(float temperature) {
  if (!isDisplayAvailable()) {
    return;
  }

  char lcd_line0[16];
  char lcd_line1[16];

  sprintf(lcd_line0, "Current: %5.1fºC", temperature);
  sprintf(lcd_line1, "Target:  %5.1fºC", targetTemperature);
  lcdPrint(lcd_line0, lcd_line1);
}


void setup(void) {
  Serial.begin(9600);
  lcdPrint("Starting up Temperature Monitor/Controller");

  sensors.begin();

  setupModeControl();
  setupRelay();
  targetTemperature = readTemperature();

  lcd.begin(16, 2);
}


void loop(void) {
  bool isSwitchPressed = digitalRead(ACTIVE_SWITCH_PIN) == HIGH;
  if (isSwitchPressed) {
    toggleActiveMode();
  }

  float temperature = readTemperature();
  lcdPrint(temperature);

  if (temperature < targetTemperature) {
    switchRelayOn();
  } else {
    switchRelayOff();
  }

  displayTemperature(temperature);
}


void serialEvent() {
  while (Serial.available()) {
    targetTemperature = (int)Serial.read();
  }
}
