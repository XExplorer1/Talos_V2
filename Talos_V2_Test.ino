#include <Arduino.h>
#include "MotorControls.h"

MotorControls robot;
#define PIN_BUTTON 0
bool running = false;   // Variable to start and stop looping in test
//////////////////////////////////////////////////////////////////////////////////////////
// MAIN

void setup() {
    Serial.begin(19200); // 115200 is default, but i've also been using 19200


    robot.SetupMotorControlsSystem();
    pinMode(PIN_BUTTON, INPUT_PULLUP);


    Serial.println("Talos Straightaway test ready.");
    Serial.println("Boot complete. Waiting for first button press...");
}
void loop() {

    On_Off_Switch(); // if off, wait till button press. if on, stop if button pressed.
    if(running){
/*
        //forward test block ///////////////////////////////////////////////////////////////
        Serial.println("About to call goForward()");
        robot.goForward(4950); // NOTE: 50 pulses = ~1cm
        Serial.println("Returned from goForward()");
    
        delay(1000);
        
        //left turn test block ///////////////////////////////////////////////////////////////
        Serial.println("About to call turnLeft(int)");
        robot.turnLeft(690);
        Serial.println("Returned from turnLeft(int)");

        delay(1000);

        //right turn test block ///////////////////////////////////////////////////////////////
        Serial.println("About to call turnRight(int)");
        robot.turnRight(690);
        Serial.println("Returned from turnRight(int)");

        delay(1000);

        //intersection left turn test block ///////////////////////////////////////////////////////////////
        Serial.println("About to call intersectionLeftTurn()");
        robot.IntersectionLeftTurn();
        Serial.println("Returned from intersectionLeftTurn()");

        delay(1000);
*/
        //intersection right turn test block ///////////////////////////////////////////////////////////////
        Serial.println("About to call gintersectionRightTurn()");
        robot.IntersectionRightTurn();
        Serial.println("Returned from intersectionRightTurn()");

        delay(1000);
/*
        //Smooth intersection left turn test block ///////////////////////////////////////////////////////////////
        Serial.println("About to call SmoothIntersectionLeftTurn()");
        robot.SmoothIntersectionLeftTurn();
        Serial.println("Returned from SmoothIntersectionLeftTurn()");

        delay(1000);

        //smooth intersection right turn test block ///////////////////////////////////////////////////////////////
        Serial.println("About to call SmoothIntersectionRightTurn()");
        robot.SmoothIntersectionRightTurn();
        Serial.println("Returned from SmoothIntersectionRightTurn()");
*/
        turnOffRunning();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS


// The function returns true only once per physical press, after confirming the button is held for ~50 ms (debounce) and then waiting until the user releases it before returning.
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
//makes the running varible swap between true and false upon pressing boot button. 
void On_Off_Switch(){
    if (!running) {
        if (buttonPressed()) {
            Serial.println("Button pressed! Starting in 1 second...");
            delay(1000);
            turnOnRunning();
        }
        return;
    }
    else {
        if (buttonPressed()) {
            Serial.println("Button pressed! Shutting down in 1 second...");
            delay(1000);
            turnOffRunning();
        }
        return;
    }
}
void turnOnRunning(){
    running = true;
    Serial.println("Running!");
}
void turnOffRunning(){
    running = false;
    Serial.println("Stopping!");
}