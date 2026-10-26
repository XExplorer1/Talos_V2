#ifndef MOTOR_CONTROLS_H
#define MOTOR_CONTROLS_H

#include <Arduino.h>
#include "Talos_Motor.h"
#include "Talos_Encoder.h"

class MotorControls {
public:
    MotorControls();

    // System initialization
    void SetupMotorControlsSystem();        // <-- use this one if IR sensors will be managed by the NAV system
    void SetupMotorControlsSystemWithIR();  // <-- use this one if IR sensors will be managed by the Motor system

    // System-wide reset (stop motors, clear flags, reset encoders)
    void ResetMotorControlsSystem();

    // Movement methods ///////////////////////////////////////////////////////////////// 
    
    // NOTE: 50 pulses = ~1cm
    void goForward(uint32_t targetPulses);      // moves forward, only stops when encoder target pulses are reached.
    void goForwardUntilIntersection();          // moves forward, only stops when stop line is detected.
    void goForwardWithIR(uint32_t targetPulses, bool enableStopBarDetection); // moves forward with lane correction, only stops when encoder target pulses are reached. if "enableStopBarDetection" is true, then also stop at the next stop line.
    
    //manual turn methods
    void turnLeft(uint32_t targetPulses);
    void turnRight(uint32_t targetPulses);
    
    //standardized intersection turns
    void IntersectionLeftTurn(); 
    void IntersectionRightTurn();
    void SmoothIntersectionLeftTurn();
    void SmoothIntersectionRightTurn();

private:
    
    // ==== Motion Constants ====
    static const uint16_t BASE_LEFT_DRIVE_PWM  = 200;   // Tune this number to calibrate motor differences 
    static const uint16_t BASE_RIGHT_DRIVE_PWM = 210;   // Tune this number to calibrate motor differences 
    static const uint16_t LANE_CORRECTION_PWM = 50;     // Tune this number to calibrate motor differences // << DOUBLE CHECK THIS, VALUE GIVEN BY AI <------------------------------------------------
    static const uint16_t MAX_PWM = 1023;               // 10-bit PWM resolution = 0-1023                  // << DOUBLE CHECK THIS, VALUE GIVEN BY AI <------------------------------------------------
    static const uint16_t DEFAULT_ACCELERATION_DELAY_MS = 10;                                              // << DOUBLE CHECK THIS, VALUE GIVEN BY AI <------------------------------------------------

    // ==== Intersection Constants ====
    static const uint32_t INTERSECTION_FORWARD_PULSES_FOR_LEFT_TURN = 500;  // ~10cm <--adjust this value
    static const uint32_t INTERSECTION_FORWARD_PULSES_FOR_RIGHT_TURN = 150; // ~3cm <--adjust this value

    static const uint32_t INTERSECTION_LEFT_TURN_PULSES = 690;  // # of pulses for a 90-degree left turn <--adjust this value
    static const uint32_t INTERSECTION_RIGHT_TURN_PULSES = 690; // # of pulses for a 90-degree right turn <--adjust this value
    // Smooth left
    static const uint32_t SMOOTH_INTERSECTION_LEFT_TURN_PULSES_LEFT_MOTOR = 700;  // # of pulses for a 90-degree left turn <--adjust this value
    static const uint32_t SMOOTH_INTERSECTION_LEFT_TURN_PULSES_RIGHT_MOTOR = 700; // # of pulses for a 90-degree right turn <--adjust this value
    // Smooth right
    static const uint32_t SMOOTH_INTERSECTION_RIGHT_TURN_PULSES_LEFT_MOTOR = 700;  // # of pulses for a 90-degree left turn <--adjust this value
    static const uint32_t SMOOTH_INTERSECTION_RIGHT_TURN_PULSES_RIGHT_MOTOR = 700; // # of pulses for a 90-degree right turn <--adjust this value

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

    void setupEncoderSystem();
    void setupIRSystem();
};

#endif
