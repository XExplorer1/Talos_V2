#include "Talos_Motor.h"
#include <driver/ledc.h>

Talos_Motor::Talos_Motor(uint8_t pwmPin, uint8_t pwmChannel, uint32_t pwmFrequency, uint8_t pwmResolution)
    : _pwmPin(pwmPin), _pwmChannel(pwmChannel)
{
    // Configure LEDC timer
    ledc_timer_config_t timerConfig = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = (ledc_timer_bit_t)pwmResolution,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = pwmFrequency,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timerConfig);

    // Configure LEDC channel
    ledc_channel_config_t channelConfig = {
        .gpio_num       = _pwmPin,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = (ledc_channel_t)_pwmChannel,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 0,
        .hpoint         = 0
    };
    ledc_channel_config(&channelConfig);
}

void Talos_Motor::setPWM(uint16_t dutyCycle)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)_pwmChannel, dutyCycle);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)_pwmChannel);
}

void Talos_Motor::stop()
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)_pwmChannel, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)_pwmChannel);
}
