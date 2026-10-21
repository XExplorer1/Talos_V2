#include "MotorControls.h"

// ==== Static Member Definitions =======================================================================================================================

volatile bool MotorControls::irFrontFlag = false;
volatile bool MotorControls::irLeftFlag  = false;
volatile bool MotorControls::irRightFlag = false;

MotorControls* MotorControls::instancePtr = nullptr;

// ==== Constructor =====================================================================================================================================

MotorControls::MotorControls() : motorLeft(MOTOR_LEFT_PWM, 0, 20000, 10), motorRight(MOTOR_RIGHT_PWM, 1, 20000, 10), encoderLeft(), encoderRight(){
    // Assign instance pointer for ISR access
    instancePtr = this;
}
// ==== Setup Functions =================================================================================================================================
void MotorControls::SetupMotorControlsSystem(){
    // Configure motor direction pins
    pinMode(MOTOR_LEFT_AIN, OUTPUT);
    pinMode(MOTOR_RIGHT_AIN, OUTPUT);

    // Forward-only operation
    digitalWrite(MOTOR_LEFT_AIN, HIGH);
    digitalWrite(MOTOR_RIGHT_AIN, HIGH);

    setupEncoderSystem();

    ResetMotorControlsSystem();
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
    attachInterrupt(
        digitalPinToInterrupt(ENC_LEFT_PIN),
        onEncoderLeftISR,
        RISING);

    attachInterrupt(
        digitalPinToInterrupt(ENC_RIGHT_PIN),
        onEncoderRightISR,
        RISING);
}

void MotorControls::setupIRSystem(){
    pinMode(IR_FRONT_PIN, INPUT);
    pinMode(IR_LEFT_PIN, INPUT);
    pinMode(IR_RIGHT_PIN, INPUT);
}

// ==== Helper Functions ================================================================================================================================

void MotorControls::enableIRInterrupts(){
    attachInterrupt(
        digitalPinToInterrupt(IR_FRONT_PIN),
        onIRFrontISR,
        RISING);

    attachInterrupt(
        digitalPinToInterrupt(IR_LEFT_PIN),
        onIRLeftISR,
        RISING);

    attachInterrupt(
        digitalPinToInterrupt(IR_RIGHT_PIN),
        onIRRightISR,
        RISING);
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
    // Start from a known state
    encoderLeft.reset();
    encoderRight.reset();

    // Start both motors
    motorLeft.setPWM(BASE_LEFT_DRIVE_PWM);
    motorRight.setPWM(BASE_RIGHT_DRIVE_PWM);

    // Continue until BOTH wheels reach target
    while (encoderLeft.getCount() < targetPulses || encoderRight.getCount() < targetPulses){
        // Intentionally empty
        // Future encoder correction logic can be added here
    }

    // Stop and clean up
    ResetMotorControlsSystem();
}

