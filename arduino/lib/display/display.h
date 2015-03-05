#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"
#include "LiquidCrystal.h"


class Display {
private:
    LiquidCrystal *lcd;

    unsigned long lcd_timer;
    bool isDisplayLocked;

    void lcdPrint(char const *line1);
    void lcdPrint(char const *line1, char const *line2);

    void lockDisplay();
    void releaseDisplay();
    bool isDisplayAvailable();

public:
    Display(LiquidCrystal*);
    // Display(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

    void begin(void);

    void displayStatusMessage(char const *message);
    void displayTemperature(float temperature, float targetTemperature);
};

#endif
