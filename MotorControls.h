#ifndef MOTOR_CONTROLS_H
#define MOTOR_CONTROLS_H

#include <Arduino.h>
#include "Talos_Motor.h"
#include "Talos_Encoder.h"

class MotorControls {
public:
    MotorControls();

    // System initialization
    void SetupMotorControlsSystem();

    // System-wide reset (stop motors, clear flags, reset encoders)
    void ResetMotorControlsSystem();

    // Movement methods
    void goForward(uint32_t targetPulses);      // moves forward, only stops when encoder target pulses are reached.
    void goForwardUntilIntersection();          // moves forward, only stops when stop line is detected.
    void goForwardWithIR(uint32_t targetPulses, bool enableStopBarDetection); // moves forward with lane correction, only stops when encoder target pulses are reached. if "enableStopBarDetection" is true, then also stop at the next stop line.

private:
    // ==== Pin Definitions ====
    static const uint8_t IR_FRONT_PIN = 14;
    static const uint8_t IR_LEFT_PIN  = 21;
    static const uint8_t IR_RIGHT_PIN = 47;

    static const uint8_t ENC_LEFT_PIN  = 1;
    static const uint8_t ENC_RIGHT_PIN = 2;

    static const uint8_t MOTOR_LEFT_AIN  = 38;
    static const uint8_t MOTOR_LEFT_PWM  = 39;

    static const uint8_t MOTOR_RIGHT_AIN = 40;
    static const uint8_t MOTOR_RIGHT_PWM = 41;

    // ==== Motor & Encoder Objects ====
    Talos_Motor motorLeft;
    Talos_Motor motorRight;

    Talos_Encoder encoderLeft;
    Talos_Encoder encoderRight;

    // ==== IR Flags ====
    static volatile bool irFrontFlag;
    static volatile bool irLeftFlag;
    static volatile bool irRightFlag;

    // Pointer used by static ISRs to access instance members
    static MotorControls* instancePtr;

    // ==== Interrupt Service Routines ====
    static void IRAM_ATTR onEncoderLeftISR();
    static void IRAM_ATTR onEncoderRightISR();

    static void IRAM_ATTR onIRFrontISR();
    static void IRAM_ATTR onIRLeftISR();
    static void IRAM_ATTR onIRRightISR();

    // ==== Helper Methods ====
    void enableIRInterrupts();
    void disableIRInterrupts();
};

#endif
