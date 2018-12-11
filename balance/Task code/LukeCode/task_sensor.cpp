//**************************************************************************************
/** \file task_sensor.cpp
 *    This file contains source code for reading an IMU.
 *
 *  Revisions:
 *    \li 11-29-2018 LEW Original file
 *
 *  Accreditation:
 *    The structure of this file, organization and some content, was directly written by
 *    John Ridgely, 11-26-2012. I have been given his files and explicit permission to
 *    adapt said files for ME 507 project use. */
//**************************************************************************************

#include "task_sensor.h"


//-------------------------------------------------------------------------------------
/** @brief   Constructor for an sensor class object.
 *  @details This constructor calls its parent class's constructor to actually do the
 *           work of creating the sensor task.
 *  @param   p_name A name for this task
 *  @param   prio The priority at which this task will run (should be low)
 *  @param   stacked The stack space to be used by the task (not much)
 *  @param   serpt A pointer to a serial device on which debugging messages are shown
 *  @param   p_polydaq2 A pointer to the PolyDAQ 2 driver used to interface with the
 *                      data acquisition board
 */

task_sensor::task_sensor (const char* p_name, unsigned portBASE_TYPE prio, size_t stacked,
					  emstream* serpt)
	: TaskBase (p_name, prio, stacked, serpt)
{
	// Save the pointer to the PolyDAQ driver, which has its own A/D and D/A drivers
}


//-------------------------------------------------------------------------------------
/** @brief   The run method that controls the effort of motors x and y.
 *  @details This method
 */

void task_sensor::run (void)
{
    Balance sensor = new Balance            // Comfortable shoeware
	for (;;)
	{
		switch (state)
		{
			// In state 0,
			case (0):
            {
                void read_sensor (void)
            }
        }; // switch (state)

		runs++;                             // Track how many runs through the loop

		delay_from_for_ms (xLastWakeTime, 1);
	}
}
