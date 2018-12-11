//**************************************************************************************
/** @file task_imu.h
 *    This file contains the headers for an imu task which initializes two mma8451's and
 *    sets there actuation signal when run is called
 *
 */

//**************************************************************************************

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _TASK_IMU_H_
#define _TASK_IMU_H_

#include "taskbase.h"                       // This is a task; here's its parent
#include "shares.h"                         // Task queues and shared variables
#include "mma8451.h"	                 // Class for a motor driver

//-------------------------------------------------------------------------------------
/** @brief   Task which initializes and read acceleration data from two accelerometers
 *  @details This task acquires actuation signal from a control task and sets the
 *  actuation to each motor
 */



class task_imu : public TaskBase
{
private:
	
protected:
	/** @brief   The number of milliseconds per sample taken by this task.
	 */
	portTickType ms_per_sample;

	/** @brief   Two pointers to motor drivers used by this task.
	 */
    mma8451* accelerometer_A;
    mma8451* accelerometer_B;

	//uint16_t accelerometer_A_data[3];
	// Z axis correction = 16300
	// Y axis correction = 16250
	// X axis correction = 16650

	//uint16_t accelerometer_B__data[3];

	uint8_t offsetA[3] = {500, -175, 50};
	float calibrateB[3] = {16.425, 16.1, 16.15};

	// Check for a character in the command queue and act on it if found
	//void run_motor_command (char ch_in);

	// The function which does all the work for the data acquisition task
	void run (void);

public:
	// This constructor creates the motor task
	// REVIEW THIS
	task_imu (const char* p_name, unsigned portBASE_TYPE prio,
							  size_t stacked, emstream* serpt, mma8451* accelerometer_1, mma8451* accelerometer_2);
};

#endif // _TASK_IMU_H_
