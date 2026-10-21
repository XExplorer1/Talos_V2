#include "MotorControls.h"

// ==== Static Member Definitions ====
volatile bool MotorControls::irFrontFlag = false;
volatile bool MotorControls::irLeftFlag  = false;
volatile bool MotorControls::irRightFlag = false;

MotorControls* MotorControls::instancePtr = nullptr;

// ==== Constructor ====
MotorControls::MotorControls()
    : motorLeft(MOTOR_LEFT_PWM, 0, 20000, 10),
      motorRight(MOTOR_RIGHT_PWM, 1, 20000, 10),
      encoderLeft(),
      encoderRight()
{
    // Assign instance pointer for ISR access
    instancePtr = this;
}

void MotorControls::SetupMotorControlsSystem()
{
    // Configure motor direction pins
    pinMode(MOTOR_LEFT_AIN, OUTPUT);
    pinMode(MOTOR_RIGHT_AIN, OUTPUT);

    // Always run motors in forward mode
    digitalWrite(MOTOR_LEFT_AIN, HIGH);
    digitalWrite(MOTOR_RIGHT_AIN, HIGH);

    // Configure encoder pins
    pinMode(ENC_LEFT_PIN, INPUT);
    pinMode(ENC_RIGHT_PIN, INPUT);

    // Configure IR sensor pins
    pinMode(IR_FRONT_PIN, INPUT);
    pinMode(IR_LEFT_PIN, INPUT);
    pinMode(IR_RIGHT_PIN, INPUT);

    // Attach encoder interrupts
    attachInterrupt(
        digitalPinToInterrupt(ENC_LEFT_PIN),
        onEncoderLeftISR,
        RISING);

    attachInterrupt(
        digitalPinToInterrupt(ENC_RIGHT_PIN),
        onEncoderRightISR,
        RISING);

    // IR interrupts remain disabled until needed
    disableIRInterrupts();

    // Ensure clean startup state
    ResetMotorControlsSystem();
}

void MotorControls::ResetMotorControlsSystem()
{
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

void MotorControls::setupEncoderSystem()
{
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

void MotorControls::setupIRSystem()
{
    pinMode(IR_FRONT_PIN, INPUT);
    pinMode(IR_LEFT_PIN, INPUT);
    pinMode(IR_RIGHT_PIN, INPUT);
}

void MotorControls::enableIRInterrupts()
{
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

void MotorControls::disableIRInterrupts()
{
    detachInterrupt(digitalPinToInterrupt(IR_FRONT_PIN));
    detachInterrupt(digitalPinToInterrupt(IR_LEFT_PIN));
    detachInterrupt(digitalPinToInterrupt(IR_RIGHT_PIN));
}

void IRAM_ATTR MotorControls::onEncoderLeftISR()
{
    if (instancePtr != nullptr)
    {
        instancePtr->encoderLeft.increment();
    }
}

void IRAM_ATTR MotorControls::onEncoderRightISR()
{
    if (instancePtr != nullptr)
    {
        instancePtr->encoderRight.increment();
    }
}

void IRAM_ATTR MotorControls::onIRFrontISR()
{
    irFrontFlag = true;
}

void IRAM_ATTR MotorControls::onIRLeftISR()
{
    irLeftFlag = true;
}

void IRAM_ATTR MotorControls::onIRRightISR()
{
    irRightFlag = true;
}