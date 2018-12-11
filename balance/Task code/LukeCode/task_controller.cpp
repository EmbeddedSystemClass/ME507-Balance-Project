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
 *  @param   p_polydaq2 A pointer to the PolyDAQ 2 driver used to interface with the
 *                      data acquisition board
 */

task_controller::task_controller (const char* p_name, unsigned portBASE_TYPE prio, size_t stacked,
					  emstream* serpt)
	: TaskBase (p_name, prio, stacked, serpt)
{
	controller = new Balance()                        // Comfortable shoeware
	// Save the pointer to the PolyDAQ driver, which has its own A/D and D/A drivers
}


//-------------------------------------------------------------------------------------
/** @brief   The run method that controls the effort of motors x and y.
 *  @details This method
 */

void task_controller::run (void)
{
	controller.set_gains (void);					// Input proportional and integral gains
	for (;;)
	{
		switch (state)
		{
			// In state 0,
			case (0):
            {
            	controller.convert();
                // Converts IMU signals to mG
                //controller.set_setpoint ();     // Acquire appropriate setpoint value
                controller.control();				    // Applies PI control to output actuation
                                                    // signal
                //saturate (void);	    			// Saturates actuation signal to duty cycle
            }
        }; // switch (state)

		runs++;                             // Track how many runs through the loop

		delay_from_for_ms (xLastWakeTime, 10);
	}
}
