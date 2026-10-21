#ifndef TALOS_MOTOR_H
#define TALOS_MOTOR_H

#include <Arduino.h>

class Talos_Motor {
public:
    Talos_Motor(uint8_t pwmPin, uint8_t pwmChannel, uint32_t pwmFrequency, uint8_t pwmResolution);

    void setPWM(uint16_t dutyCycle); // 10-bit → 0–1023
    void stop();

private:
    uint8_t _pwmPin;
    uint8_t _pwmChannel;
};

#endif
