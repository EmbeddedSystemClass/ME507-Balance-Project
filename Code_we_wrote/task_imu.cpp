//**************************************************************************************
/** \file task_imu.cpp
 *    This file contains the source for an imu task which initializes two mma8451's and
 *    save there data to a share
 *
 */
//**************************************************************************************

#include "task_imu.h"

//-------------------------------------------------------------------------------------
/** @brief   This constructor creates an imu task.
 *  @param   p_name A name for this task
 *  @param   prio The priority at which this task will run
 *  @param   stacked The stack space to be used by the task
 *  @param   serpt A pointer to a serial device on which debugging messages are shown
 *  @param   accelerometerIn A pointer to an accelerometer
 */

task_imu::task_imu (const char* p_name, unsigned portBASE_TYPE prio,
							  size_t stacked, emstream* serpt, mma8452q* accelerometerIn)
	: TaskBase (p_name, prio, stacked, serpt)
{
	// Initializes class variables
	accelerometer = accelerometerIn;
}

//-------------------------------------------------------------------------------------
/** @brief   The run method that runs the motor task code.
 *  @details This method sets the actuation signal of each motor
 */

void task_imu::run (void)
{
	// This counter is used to run through the for (;;) loop at precise intervals
 	TickType_t LastWakeTime = xTaskGetTickCount ();
 	uint8_t i;
 	uint8_t dataIndex = 0;
 	// buffer struct used in task share
 	accelBuf buffer;

 	// In the main loop, read the accelerometer data and set it to the next available
	// index of the task share structure
	for (;;)
	{
		// get the old accelerometer data to be updated
	    buffer = accelerometer_A_data->get();
	    for (i = 0; i<3; i++)
	    {
            buffer.accel_buffer[dataIndex].data[i] =
                    (accelerometer->get_one_axis(i) - offsetA[i]) / calibrateA[i];
        }
        dataIndex++;
        if(dataIndex > 5)
        {
            dataIndex = 0;
        }
        // Put the data back in the task share
        accelerometer_A_data->put(buffer);

        runs++;                                 // Track how many runs through the loop
        delay_from_for_ms (LastWakeTime, 5);    // Timing for 200Hz
	}
}
