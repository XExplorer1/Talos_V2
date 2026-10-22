#include "Talos_Motor.h"

Talos_Motor::Talos_Motor(
    uint8_t pwmPin,
    uint8_t pwmChannel,
    uint32_t pwmFrequency,
    uint8_t pwmResolution)
    : _pwmPin(pwmPin),
      _pwmChannel(pwmChannel)
{
    // Configure PWM on this pin
    ledcAttach(_pwmPin, pwmFrequency, pwmResolution);
}

void Talos_Motor::setPWM(uint16_t dutyCycle)
{
    ledcWrite(_pwmPin, dutyCycle);
}

void Talos_Motor::stop()
{
    ledcWrite(_pwmPin, 0);
}