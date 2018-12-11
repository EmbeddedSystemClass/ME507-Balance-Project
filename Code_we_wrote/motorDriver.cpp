//**************************************************************************************
/** \file motorDriver.cpp
 *    This file contains the source for a motor driver class which uses two pwm signals
 *    and an enable pin to actuate a motor.  The actuation signal represents the duty
 *    cycle of the motor, which is set to be between -100 and 100.  PWM is given to the
 *    motor lead pins, with only one being set at a time, and the other being set to 0%
 *    duty cycle.
 *
 */
//**************************************************************************************

#include "motorDriver.h"


//-------------------------------------------------------------------------------------
/** @brief   The constructor for a Motor object.
 *  @details This constructor initializes the class variables and enables the motors
 *  EN pin
 *  @param   motor_In_1 A hw_pwm pointer which is initialized for one of the motor
 *  leads and has a duty cycle resolution of 100
 *  @param   motor_In_2 A hw_pwm pointer initialized as above for the other motor lead
 *  @param   In_1_chan The channel associated with motor_In_1 for setting the duty cycle
 *  @param   In_2_chan The channel associated with motor_In_2 for setting the duty cycle
 *  @param   EN_pin_num The GPIO_Pin_# for the EN pin
 *  @param   EN_port The GPIOX port for the EN pin where X represents the letter of the pin
 *  @param   RCC_AHB1Periph A value associated with initializing the EN pin which uses the
 *  port.  Ex: RCC_AHB1Periph_GPIOA for EN pin in port A
 */
Motor:: Motor(hw_pwm* motor_In_1, hw_pwm* motor_In_2, uint8_t In_1_chan, uint8_t In_2_chan,
        uint16_t EN_pin_num, GPIO_TypeDef* EN_port, uint32_t RCC_AHB1Periph)
{
    // Initialize class variables
    motor_IN1 = motor_In_1;
    motor_IN2 = motorIn_2;
    IN2_chan = In_2_chan;
    IN1_chan = In_1_chan;

    // Initialize Port Clock for Motor EN pin
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph, ENABLE);

    // Initialize the I/O port pin EN for motor
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = EN_pin_num;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // DONT NEED ALTERNATE FUNC FOR THIS?
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // push pull
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
    GPIO_Init (GPIOA, &GPIO_InitStruct);

    // Set EN pin on
    GPIO_SetBits(EN_port, EN_pin_num);
}


/** @brief   A method which uses the actuation signal to set a duty cycle.
 *  @details The function saturates the actuation signal to between -100 and 100
 *  meaning that the resolution of the motor_IN objects must be 100.
 *  @param   actuationSignal The signed value of duty cycle to be set for the motor.
 *  A positive value will set motor_IN1 and a negative value will set motor_IN2.
 */
void Motor :: setActuation(int16_t actuationSignal)
{
    // Ensure that the actuation signal is below 100% assuming MOTOR_MAX_PWM = 100
    if (actuationSignal > 100)
        actuationSignal = 100;
    if (actuationSignal < -100)
        actuationSignal = -100;

    // set up pins of motor for pwm and set pulse width according to actuation signal
    if (actuationSignal > 0){
        motor_IN2->set_duty_cycle(IN2_chan, 0);
        motor_IN1->set_duty_cycle(IN1_chan, actuationSignal);
    }
    else{
        motor_IN1->set_duty_cycle(IN1_chan, 0);
        motor_IN2->set_duty_cycle(IN2_chan, -1 * actuationSignal);
    }

}
