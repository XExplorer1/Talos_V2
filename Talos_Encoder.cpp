#include "Talos_Encoder.h"

Talos_Encoder::Talos_Encoder()
    : _pulseCount(0)
{
}

void Talos_Encoder::increment()
{
    _pulseCount++;
}

void Talos_Encoder::reset()
{
    _pulseCount = 0;
}

uint32_t Talos_Encoder::getCount() const
{
    return _pulseCount;
}
