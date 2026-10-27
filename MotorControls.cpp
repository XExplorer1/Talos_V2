#include "MotorControls.h"

// ==== Static Member Definitions =======================================================================================================================

volatile bool MotorControls::irFrontFlag = false;
volatile bool MotorControls::irLeftFlag  = false;
volatile bool MotorControls::irRightFlag = false;

MotorControls* MotorControls::instancePtr = nullptr;

// ==== Constructor =====================================================================================================================================

MotorControls::MotorControls()
    : motorLeft(MOTOR_LEFT_PWM, 0, 20000, 8),
      motorRight(MOTOR_RIGHT_PWM, 1, 20000, 8),
      encoderLeft(),
      encoderRight()
{
    instancePtr = this;
}
// ==== Setup Functions =================================================================================================================================
void MotorControls::SetupMotorControlsSystem(){
    // Configure motor direction pins
    pinMode(MOTOR_LEFT_AIN, OUTPUT);
    pinMode(MOTOR_RIGHT_AIN, OUTPUT);

    // Forward-only operation
    digitalWrite(MOTOR_LEFT_AIN, LOW);
    digitalWrite(MOTOR_RIGHT_AIN, LOW);

    setupEncoderSystem();

    ResetMotorControlsSystem();

    Serial.print("Left AIN = ");
    Serial.println(digitalRead(MOTOR_LEFT_AIN));

    Serial.print("Right AIN = ");
    Serial.println(digitalRead(MOTOR_RIGHT_AIN));
}

void MotorControls::SetupMotorControlsSystemWithIR(){
    // Configure motor direction pins
    pinMode(MOTOR_LEFT_AIN, OUTPUT);
    pinMode(MOTOR_RIGHT_AIN, OUTPUT);

    // Forward-only operation
    digitalWrite(MOTOR_LEFT_AIN, HIGH);
    digitalWrite(MOTOR_RIGHT_AIN, HIGH);

    setupEncoderSystem();
    setupIRSystem();

    ResetMotorControlsSystem();

    Serial.print("Left AIN = ");
    Serial.println(digitalRead(MOTOR_LEFT_AIN));

    Serial.print("Right AIN = ");
    Serial.println(digitalRead(MOTOR_RIGHT_AIN));
}

void MotorControls::ResetMotorControlsSystem(){
    // Stop both motors
    motorLeft.stop();
    motorRight.stop();

    // Reset encoder counts
    encoderLeft.reset();
    encoderRight.reset();

    // Clear IR event flags
    irFrontFlag = false;
    irLeftFlag  = false;
    irRightFlag = false;

    // Disable IR interrupts
    disableIRInterrupts();
}

void MotorControls::setupEncoderSystem(){
    // Configure encoder pins
    pinMode(ENC_LEFT_PIN, INPUT);
    pinMode(ENC_RIGHT_PIN, INPUT);

    // Attach encoder interrupts
    attachInterrupt(digitalPinToInterrupt(ENC_LEFT_PIN), onEncoderLeftISR, RISING);

    attachInterrupt(digitalPinToInterrupt(ENC_RIGHT_PIN), onEncoderRightISR, RISING);
}

void MotorControls::setupIRSystem(){
    pinMode(IR_FRONT_PIN, INPUT);
    pinMode(IR_LEFT_PIN, INPUT);
    pinMode(IR_RIGHT_PIN, INPUT);
}

// ==== Helper Functions ================================================================================================================================

void MotorControls::enableIRInterrupts(){
    attachInterrupt(digitalPinToInterrupt(IR_FRONT_PIN), onIRFrontISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(IR_LEFT_PIN), onIRLeftISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(IR_RIGHT_PIN), onIRRightISR, FALLING);
}

void MotorControls::disableIRInterrupts(){
    detachInterrupt(digitalPinToInterrupt(IR_FRONT_PIN));
    detachInterrupt(digitalPinToInterrupt(IR_LEFT_PIN));
    detachInterrupt(digitalPinToInterrupt(IR_RIGHT_PIN));
}

// ==== Interupt Methods ================================================================================================================================

void IRAM_ATTR MotorControls::onEncoderLeftISR(){
    if (instancePtr != nullptr)
    {
        instancePtr->encoderLeft.increment();
    }
}

void IRAM_ATTR MotorControls::onEncoderRightISR(){
    if (instancePtr != nullptr)
    {
        instancePtr->encoderRight.increment();
    }
}

void IRAM_ATTR MotorControls::onIRFrontISR(){
    irFrontFlag = true;
}

void IRAM_ATTR MotorControls::onIRLeftISR(){
    irLeftFlag = true;
}

void IRAM_ATTR MotorControls::onIRRightISR(){
    irRightFlag = true;
}

// ==== Public Methods =================================================================================================================================

void MotorControls::goForward(uint32_t targetPulses){
    Serial.println("goForward entered");
    
    // Start from a known state
    encoderLeft.reset();
    encoderRight.reset();
    uint32_t lastLeft = 0;
    uint32_t lastRight = 0;

    // Start both motors
    motorLeft.setPWM(BASE_LEFT_DRIVE_PWM);
    motorRight.setPWM(BASE_RIGHT_DRIVE_PWM);

    Serial.println("PWM applied");
    Serial.print("Left Encoder Count: ");
    Serial.println(encoderLeft.getCount());
    Serial.print("Right Encoder Count: ");
    Serial.println(encoderRight.getCount());

    // Continue until BOTH wheels reach target
    while (encoderLeft.getCount() < targetPulses || encoderRight.getCount() < targetPulses){
        if (encoderLeft.getCount() != lastLeft || encoderRight.getCount() != lastRight){
                lastLeft = encoderLeft.getCount();
                lastRight = encoderRight.getCount();

                Serial.print("L: ");
                Serial.print(lastLeft);
                Serial.print("  R: ");
                Serial.println(lastRight);
            }
            delay(1);
    }

    // Stop and clean up
    ResetMotorControlsSystem();
}

void MotorControls::turnRight(uint32_t targetPulses){
    Serial.println("turnRight entered");

    // Start from a known state
    encoderLeft.reset();
    encoderRight.reset();

    // Stop right wheel
    motorRight.stop();

    // Drive left wheel
    motorLeft.setPWM(BASE_LEFT_DRIVE_PWM);

    while (encoderLeft.getCount() < targetPulses){
        delay(1);
    }

    Serial.println("turnRight target reached");

    ResetMotorControlsSystem();
}

void MotorControls::turnLeft(uint32_t targetPulses){
    Serial.println("turnLeft entered");

    // Start from a known state
    encoderLeft.reset();
    encoderRight.reset();

    // Stop left wheel
    motorLeft.stop();

    // Drive right wheel
    motorRight.setPWM(BASE_RIGHT_DRIVE_PWM);

    while (encoderRight.getCount() < targetPulses){
        delay(1);
    }

    Serial.println("turnLeft target reached");

    ResetMotorControlsSystem();
}

void MotorControls::IntersectionLeftTurn(){
    goForward(INTERSECTION_FORWARD_PULSES_FOR_LEFT_TURN);
    delay(500);
    turnLeft(INTERSECTION_LEFT_TURN_PULSES);
    delay(500);
    goForward(INTERSECTION_FORWARD_PULSES_FOR_LEFT_TURN);
}

void MotorControls::IntersectionRightTurn(){
    goForward(INTERSECTION_FORWARD_PULSES_FOR_RIGHT_TURN);
    delay(500);
    turnRight(INTERSECTION_RIGHT_TURN_PULSES);
    delay(500);
    goForward(INTERSECTION_FORWARD_PULSES_FOR_RIGHT_TURN);
}

void MotorControls::SmoothIntersectionLeftTurn(){
    Serial.println("SmoothIntersectionLeftTurn() entered");

    // Start from a known state
    encoderLeft.reset();
    encoderRight.reset();

    uint32_t lastLeft = 0;
    uint32_t lastRight = 0;

    bool leftMotorStopped = false;
    bool rightMotorStopped = false;

    // Start both motors at different speeds to create an arc
    int arcPWM = BASE_LEFT_DRIVE_PWM - SMOOTH_LEFT_TURN_LEFT_PWM_REDUCTION;
    Serial.print("arcPWM = ");
    Serial.println(arcPWM);
    motorLeft.setPWM(arcPWM);
    motorRight.setPWM(BASE_RIGHT_DRIVE_PWM);

    Serial.println("PWM applied");
    Serial.print("Left Encoder Count: ");
    Serial.println(encoderLeft.getCount());
    Serial.print("Right Encoder Count: ");
    Serial.println(encoderRight.getCount());

    while (encoderLeft.getCount() < SMOOTH_INTERSECTION_LEFT_TURN_PULSES_LEFT_MOTOR || encoderRight.getCount() < SMOOTH_INTERSECTION_LEFT_TURN_PULSES_RIGHT_MOTOR){
        // Stop left wheel once its target is reached
        if (!leftMotorStopped && encoderLeft.getCount() >= SMOOTH_INTERSECTION_LEFT_TURN_PULSES_LEFT_MOTOR){
            motorLeft.stop();
            leftMotorStopped = true;

            Serial.println("Left motor target reached");
        }

        // Stop right wheel once its target is reached
        if (!rightMotorStopped && encoderRight.getCount() >= SMOOTH_INTERSECTION_LEFT_TURN_PULSES_RIGHT_MOTOR){
            motorRight.stop();
            rightMotorStopped = true;

            Serial.println("Right motor target reached");
        }

        if (encoderLeft.getCount() != lastLeft || encoderRight.getCount() != lastRight){
            lastLeft = encoderLeft.getCount();
            lastRight = encoderRight.getCount();

            Serial.print("L: ");
            Serial.print(lastLeft);
            Serial.print("  R: ");
            Serial.println(lastRight);
        }

        delay(1);
    }

    ResetMotorControlsSystem();
}


void MotorControls::SmoothIntersectionRightTurn(){
    Serial.println("SmoothIntersectionRightTurn() entered");

    // Start from a known state
    encoderLeft.reset();
    encoderRight.reset();

    uint32_t lastLeft = 0;
    uint32_t lastRight = 0;

    bool leftMotorStopped = false;
    bool rightMotorStopped = false;

    // Start both motors at different speeds to create an arc
    int arcPWM = BASE_RIGHT_DRIVE_PWM - SMOOTH_RIGHT_TURN_RIGHT_PWM_REDUCTION;
    Serial.print("arcPWM = ");
    Serial.println(arcPWM);
    motorLeft.setPWM(BASE_LEFT_DRIVE_PWM);
    motorRight.setPWM(arcPWM);

    Serial.println("PWM applied");
    Serial.print("Left Encoder Count: ");
    Serial.println(encoderLeft.getCount());
    Serial.print("Right Encoder Count: ");
    Serial.println(encoderRight.getCount());

    while (encoderLeft.getCount() < SMOOTH_INTERSECTION_LEFT_TURN_PULSES_LEFT_MOTOR || encoderRight.getCount() < SMOOTH_INTERSECTION_LEFT_TURN_PULSES_RIGHT_MOTOR){
        // Stop left wheel once its target is reached
        if (!leftMotorStopped && encoderLeft.getCount() >= SMOOTH_INTERSECTION_LEFT_TURN_PULSES_LEFT_MOTOR){
            motorLeft.stop();
            leftMotorStopped = true;

            Serial.println("Left motor target reached");
        }

        // Stop right wheel once its target is reached
        if (!rightMotorStopped && encoderRight.getCount() >= SMOOTH_INTERSECTION_LEFT_TURN_PULSES_RIGHT_MOTOR){
            motorRight.stop();
            rightMotorStopped = true;

            Serial.println("Right motor target reached");
        }

        if (encoderLeft.getCount() != lastLeft || encoderRight.getCount() != lastRight){
            lastLeft = encoderLeft.getCount();
            lastRight = encoderRight.getCount();

            Serial.print("L: ");
            Serial.print(lastLeft);
            Serial.print("  R: ");
            Serial.println(lastRight);
        }

        delay(1);
    }

    ResetMotorControlsSystem();
}

