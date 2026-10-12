#include <Arduino.h>
#include "MotorControls.h"

#define PIN_BUTTON  0

MotorControls motors;

//////////////////////////////////////////////////////////////////////////////////////////
// MAIN

void setup() {

}
void loop() {

}

//////////////////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS

bool buttonPressed() {
    if (digitalRead(PIN_BUTTON) == LOW) {
        delay(50);
        if (digitalRead(PIN_BUTTON) == LOW) {
            while (digitalRead(PIN_BUTTON) == LOW) {}
            return true;
        }
    }
    return false;
}