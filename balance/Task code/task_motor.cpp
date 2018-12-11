//**************************************************************************************
/** \file task_data_acq.cpp
 *    This file contains the source for a motor task which initializes two motors and
 *    sets there actuation signal when run is called
 *
 */
//**************************************************************************************

// #include "stdint.h"
#include "emstream.h"
#include "task_motor.h"


//-------------------------------------------------------------------------------------
/** @brief   This constructor creates a motor task.
 *  @param   p_name A name for this task
 *  @param   prio The priority at which this task will run (should be low)
 *  @param   stacked The stack space to be used by the task (not much)
 *  @param   serpt A pointer to a serial device on which debugging messages are shown
 *  @param   p_polydaq2 A pointer to the PolyDAQ 2 driver used to interface with the
 *                      data acquisition board
 */

task_motor::task_motor (const char* p_name, unsigned portBASE_TYPE prio,
							  size_t stacked, emstream* serpt, Motor* motor1, Motor* motor2)
	: TaskBase (p_name, prio, stacked, serpt)
{
	// Initialize both motors with constructor
	motorA = motor1;
	motorB = motor2;



	// By default, take samples once per second if in continuous sampling mode
	//ms_per_sample = 100;

	// No A/D channel has been read yet
	//last_data_channel = (char)0xFF;
}


//-------------------------------------------------------------------------------------
/** @brief   The run method that runs the motor task code.
 *  @details This method sets the actuation signal of each motor
 */

void task_motor::run (void)
{
    //

// 	uint16_t ms_counter = 0;                // Counts ms per sample

	// This counter is used to run through the for (;;) loop at precise intervals
 	TickType_t LastWakeTime = xTaskGetTickCount ();

	// In the main loop, wait for a command through the command queue; if a command
	// is received, do what the commander asked
	for (;;)
	{
		motorA->setActuation(motor_A_actuation_signal);// info from share
		motorB->setActuation(motor_B_actuation_signal);// info from share
		runs++;                             // Track how many runs through the loop
 		delay_from_for_ms (LastWakeTime, 10);    // Always a 1 ms delay for this task
	}
}
