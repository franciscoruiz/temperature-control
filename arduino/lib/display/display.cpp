#include "Arduino.h"
#include "LiquidCrystal.h"
#include "display.h"


#define STATUS_MESSAGE_MILLIS 2000


/*
 * Constructor
 */
Display::Display(LiquidCrystal *lcd) {
  this->lcd = lcd;
}


/*
 * Public
 */
void Display::begin(void) {
  lcd->begin(16, 2);
}


void Display::displayStatusMessage(char const *message) {
  lockDisplay();
  lcdPrint(message);
}


void Display::displayTemperature(float temperature, float targetTemperature) {
  if (!isDisplayAvailable()) {
    return;
  }

  char lcd_line0[16];
  char lcd_line1[16];

  sprintf(lcd_line0, "Current: %d", (int)temperature);
  sprintf(lcd_line1, "Target:  %d", (int)targetTemperature);

  lcdPrint(lcd_line0, lcd_line1);
}


/*
 * Private
 */
void Display::lcdPrint(char const *line1) {
  lcdPrint(line1, "");
}


void Display::lcdPrint(char const *line1, char const *line2) {
  lcd->clear();

  lcd->setCursor(0, 0);
  lcd->print(line1);

  lcd->setCursor(0, 1);
  lcd->print(line2);
}


void Display::lockDisplay () {
  isDisplayLocked = true;
  lcd_timer = millis();
}

void Display::releaseDisplay() {
  isDisplayLocked = false;
}

bool Display::isDisplayAvailable() {
  if (!isDisplayLocked) {
    return true;
  }

  bool isTimerExpired = millis() - lcd_timer > STATUS_MESSAGE_MILLIS;
  if (isTimerExpired) {
    releaseDisplay();
  }
  return isTimerExpired;
}
