#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "hw_pwm.h"

/** NEED TO CHANGE THIS MOTOR TASK SO THAT IT TAKES IN A TIMER CHANNEL
    two pins, a port,
    */

class Motor
{
protected:
    hw_pwm* motor_IN1;
    hw_pwm* motor_IN2;
    uint8_t IN1_chan;
    uint8_t IN2_chan;
public:
    Motor();
    Motor(hw_pwm* motor_IN1, hw_pwm* motor_IN2, uint8_t In_1_chan, uint8_t In_2_chan);
    void setActuation(int16_t actuationSignal);
};
#endif
