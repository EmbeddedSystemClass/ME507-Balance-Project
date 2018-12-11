//**************************************************************************************
/** \file task_motor.cpp
 *    This file contains the source for a motor task which is a child of basetask. It
 *    takes an initialized motor driver and a motor value and sets the actuatuon of the
 *    motor using a task share.  The motor value distinguishes which task share to use.
 */
//**************************************************************************************

#include "emstream.h"
#include "task_motor.h"


//-------------------------------------------------------------------------------------
/** @brief   This constructor creates a motor task.
 *  @param   p_name A name for this task
 *  @param   prio The priority at which this task will run (should be low)
 *  @param   stacked The stack space to be used by the task (not much)
 *  @param   serpt A pointer to a serial device on which debugging messages are shown
 *  @param   motor_in A pointer to a motor driver
 *  @param   motor_num A value to distinguish which task share to use. 0 = motorA 1 = motorB
 */

task_motor::task_motor (const char* p_name, unsigned portBASE_TYPE prio,
    size_t stacked, emstream* serpt, Motor* motor_in, uint8_t motor_num)
	: TaskBase (p_name, prio, stacked, serpt)
{
	// Initialize motor and motor value constructor
	motor = motor_in;
	motor_val = motor_num;
}


//-------------------------------------------------------------------------------------
/** @brief   The run method that runs the motor task code.
 *  @details This method sets the actuation signal of the motor from the appropriate task
 *  share
 */

void task_motor::run (void)
{
	// This counter is used to run through the for (;;) loop at precise intervals
 	TickType_t LastWakeTime = xTaskGetTickCount ();

	// In the main loop, set actuation signal of the motor from a task share
	for (;;)
	{
	    if(!motor_val)
        {
            motor->setActuation(motor_A_actuation_signal->get());
        }
        else
        {
            motor->setActuation(motor_B_actuation_signal->get());
        }
		runs++;                             // Track how many runs through the loop
 		delay_from_for_ms (LastWakeTime, 10);    // delay to allow for 100Hz timig
	}
}
