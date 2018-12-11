#include "motorDriver.h"
#include "hw_pwm.h"

Motor:: Motor(hw_pwm* motor_In_1, hw_pwm* motorIn_2, uint8_t In_1_chan, uint8_t In_2_chan)
{
    motor_IN1 = motor_In_1;
    motor_IN2 = motorIn_2;
    IN2_chan = In_2_chan;
    IN1_chan = In_1_chan;
}
    // This function will become generic so that the following can be passed to it
    // The EN pin number/port
    // The IN1 IN2 pins/port
    // The timer for the IN1/IN2 pins

    // The function names (this part of it makes it difficult, unless we have a long
    // list of arguments


    //printPins();

    /*
    // Enable the clock to the GPIO port
    RCC_AHB1PeriphClockCmd (MOTOR_CLOCK, ENABLE);

    // Initialize the I/O port pin ENa of motor 1
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = MOTOR_ENA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // DONT NEED ALTERNATE FUNC FOR THIS?
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // push pull
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
    GPIO_Init (MOTOR_EN_PORT, &GPIO_InitStruct);


    // Initialize the I/O port pin IN1A of motor 1
    GPIO_InitTypeDef GPIO_InitStruct1;
    GPIO_InitStruct1.GPIO_Pin = MOTOR_A_IN1_PIN;
    GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_AF; // alternate function setting (timer)
    GPIO_InitStruct1.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
    GPIO_InitStruct1.GPIO_OType = GPIO_OType_PP; // push pull
    GPIO_InitStruct1.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
    GPIO_Init (MOTOR_IN1_IN2_PORT, &GPIO_InitStruct1);

    // Initialize the I/O port pin IN2A of motor 1
    GPIO_InitTypeDef GPIO_InitStruct2;
    GPIO_InitStruct2.GPIO_Pin = MOTOR_A_IN2_PIN;
    GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_AF; // alternate function setting (timer)
    GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
    GPIO_InitStruct2.GPIO_OType = GPIO_OType_PP; // push pull
    GPIO_InitStruct2.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
    GPIO_Init (MOTOR_IN1_IN2_PORT, &GPIO_InitStruct2);

    // Connect the port pin to its alternate source, which is the PWM
    GPIO_PinAFConfig (MOTOR_IN1_IN2_PORT, MOTOR_SOURCE1, GPIO_AF_TIM1);

    // Connect the port pin to its alternate source, which is the PWM
    GPIO_PinAFConfig (MOTOR_IN1_IN2_PORT, MOTOR_SOURCE2, GPIO_AF_TIM1);

    // Enable the clock to the timer which will be used by the PWM
    RCC_APB1PeriphClockCmd (MOTOR_CLOCK, ENABLE);


    // UNKNOWN USE
    // Compute the timer prescaler value.  FIXME:  Get that magic 21M out of here
    uint32_t PrescalerValue = (uint16_t)((SystemCoreClock / 2) / 21000000) - 1;

    // Set up the time base for the timer TIMER 1
    TIM_TimeBaseInitTypeDef TimeBaseStruct;
    TimeBaseStruct.TIM_Period = MOTOR_MAX_PWM;
    TimeBaseStruct.TIM_Prescaler = PrescalerValue;
    TimeBaseStruct.TIM_ClockDivision = 0;
    TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit (MOTOR_TIMER, &TimeBaseStruct);


    // Configure the output compare used by the PWM
    TIM_OCInitTypeDef OCInitStruct;
    OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    OCInitStruct.TIM_Pulse = 0;
    OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

    // PWM1 Mode configuration for UNKNOWN USE //Channel X (GPIOY Pin Z)
    MOTOR_OC_INIT_FN (MOTOR_TIMER, &OCInitStruct);
    MOTOR_OC_PRNF (MOTOR_TIMER, TIM_OCPreload_Enable);

    // Set the initial duty cycle to zero
    MOTOR_A_IN1_SET_DUTY (MOTOR_TIMER, 0);
    MOTOR_A_IN2_SET_DUTY (MOTOR_TIMER, 0);

    // Enable the timer; this really gets things going
    TIM_Cmd (MOTOR_TIMER, ENABLE);
}
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
        motor_IN2->set_duty_cycle(IN2_chan, actuationSignal);
    }

}
