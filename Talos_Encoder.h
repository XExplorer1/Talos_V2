#ifndef TALOS_ENCODER_H
#define TALOS_ENCODER_H

#include <Arduino.h>

class Talos_Encoder {
public:
    Talos_Encoder();

    void increment();      // called from ISR
    void reset();          // reset pulse count
    uint32_t getCount() const;

private:
    volatile uint32_t _pulseCount;
};

#endif
