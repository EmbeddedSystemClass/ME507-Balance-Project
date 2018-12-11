//**************************************************************************************
/** \file task_controller.cpp
 *    This file contains source code for a motor controller. This task serves to output
 *    the appropriate duty cycle corresponding to the effort of the motor necessary to
 *    correct the position of the platform.
 *
 *  Revisions:
 *    \li 11-29-2018 LEW Original file
 *
 *  Accreditation:
 *    The structure of this file, organization and some content, was directly written by
 *    John Ridgely, 11-26-2012. I have been given his files and explicit permission to
 *    adapt said files for ME 507 project use.
 *
 */
//**************************************************************************************

#include "task_controller.h"


//-------------------------------------------------------------------------------------
/** @brief   Constructor for a controller object.
 *  @details This constructor calls its parent class's constructor to actually do the
 *           work of creating the controller task.
 *  @param   p_name A name for this task
 *  @param   prio The priority at which this task will run (should be low)
 *  @param   stacked The stack space to be used by the task (not much)
 *  @param   serpt A pointer to a serial device on which debugging messages are shown
 *  @param   balance_controller A pointer to a Balance controller used to call functions
 *  to determine the actuation signal
 */

task_controller::task_controller (const char* p_name, unsigned portBASE_TYPE prio, size_t stacked,
					  emstream* serpt, Balance* balance_controller)
	: TaskBase (p_name, prio, stacked, serpt)
{
	controller = balance_controller;
}

//-------------------------------------------------------------------------------------
/** @brief   The run method that calculates the actuation signal of motors x and y.
 *  @details This method
 */

void task_controller::run (void)
{
	// This counter is used to run through the for (;;) loop at precise intervals
 	TickType_t xLastWakeTime = xTaskGetTickCount ();
 	accelBuf buffer = accelerometer_A_data->get();
	// Input proportional and integral gains
	controller->set_gains();
	for (;;)
	{
        // Only uses one accelerometer at this time
		buffer = accelerometer_A_data->get();
		controller->convert(buffer);           // Averages the prevous 5 acceleration values
 		controller->control();           		// Applies PI control to output actuation
		runs++;                                // Track how many runs through the loop
		delay_from_for_ms (xLastWakeTime, 10);
	}
}
