//*************************************************************************************
/** @file motorDriver.h
 *    This file contains a class for a motor driver which uses two pwm pins and an
 *    enable pin to control a motor.  The actuation signal is saturated to between
 *    100 and -100 which corresponds to a forwards and backwards 100% duty cycle.
 *    Pwm signal is only output from one pin at a time (other pin is 0% duty cycle)
 */

#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "hw_pwm.h"               // Header for pwm driver for motor pins

class Motor
{
protected:
    /** @brief A pwm configured pin for a motor lead
    */
    hw_pwm* motor_IN1;
    /** @brief A pwm configured pin for a motor lead
     */
    hw_pwm* motor_IN2;
    /** @brief The timer channel for the first pwm pin
     */
    uint8_t IN1_chan;
    /** @brief The timer channel for the second pwm pin
     */
    uint8_t IN2_chan;
public:
    /** @brief The constructor which initializes class variables and sets the EN pin
     */
    Motor(hw_pwm* motor_IN1, hw_pwm* motor_IN2, uint8_t In_1_chan, uint8_t In_2_chan,
            uint16_t EN_pin_num, GPIO_TypeDef* EN_port, uint32_t RCC_AHB1Periph);
    /** @brief The function that saturates and sets the duty cycle for the motor
     */
    void setActuation(int16_t actuationSignal);
};
#endif
