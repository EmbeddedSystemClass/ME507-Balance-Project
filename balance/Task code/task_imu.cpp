//**************************************************************************************
/** \file task_imu.cpp
 *    This file contains the source for an imu task which initializes two mma8451's and
 *    save there data to a share
 *
 */
//**************************************************************************************

// #include "stdint.h"
#include "emstream.h"
#include "task_motor.h"


//-------------------------------------------------------------------------------------
/** @brief   This constructor creates an imu task.
 *  @param   p_name A name for this task
 *  @param   prio The priority at which this task will run (should be low)
 *  @param   stacked The stack space to be used by the task (not much)
 *  @param   serpt A pointer to a serial device on which debugging messages are shown
 *  @param   accelerometer1 A pointer to an accelerometer
 */

task_imu::task_imu (const char* p_name, unsigned portBASE_TYPE prio,
							  size_t stacked, emstream* serpt, mma8451* accelerometer_1, mma8451* accelerometer_2)
	: TaskBase (p_name, prio, stacked, serpt)
{
	// Initialize both motors with constructor
	accelerometer_A = accelerometer_1;
	accelerometer_B = accelerometer_2;


	// By default, take samples once per second if in continuous sampling mode
	//ms_per_sample = 100;

	// No A/D channel has been read yet
	//last_data_channel = (char)0xFF;
}


//-------------------------------------------------------------------------------------
/** @brief   The run method that runs the motor task code.
 *  @details This method sets the actuation signal of each motor
 */

void task_imu::run (void)
{
    //

// 	uint16_t ms_counter = 0;                // Counts ms per sample

	// This counter is used to run through the for (;;) loop at precise intervals
 	TickType_t LastWakeTime = xTaskGetTickCount ();

 	uint8_t i;
 	uint8_t dataIndex;
	// In the main loop, wait for a command through the command queue; if a command
	// is received, do what the commander asked
	for (;;)
	{
	    for (i = 0; i<=2; i++)
        accelerometer_data->accel_buffer[dataIndex].accelerometer_A_data[i] = (accelerometer_A->get_one_axis(i) - offsetA[i])/calibration[i];
        accelerometer_data->accel_buffer[dataIndex].accelerometer_B_data[i] = (accelerometer_B->get_one_axis(i) - offsetB[i])/calibration[i];
        dataIndex++
        if(dataIndex > 10)
		{
        	dataIndex = 0;
		}
		runs++;                             // Track how many runs through the loop
 		delay_from_for_ms (LastWakeTime, 1);    // Always a 1 ms delay for this task
	}
}
