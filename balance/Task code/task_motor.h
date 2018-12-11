//**************************************************************************************
/** @file task_data_acq.h
 *    This file contains the headers for a motor task which initializes two motors and
 *    sets there actuation signal when run is called
 *
 */

//**************************************************************************************

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _TASK_MOTOR_H_
#define _TASK_MOTOR_H_

#include "taskbase.h"                       // This is a task; here's its parent
#include "shares.h"                         // Task queues and shared variables
#include "motorDriver.h"	                    // Class for a motor driver

//-------------------------------------------------------------------------------------
/** @brief   Task which initializes and sets actuation signals to two motors
 *  @details This task acquires actuation signal from a control task and sets the
 *  actuation to each motor
 */

class task_motor : public TaskBase
{
private:
	
protected:
	/** @brief   The number of milliseconds per sample taken by this task.
	 */
	portTickType ms_per_sample;

	/** @brief   Two pointers to motor drivers used by this task.
	 */
    Motor motorA;
    Motor motorB;


	// Check for a character in the command queue and act on it if found
	//void run_motor_command (char ch_in);

	// The function which does all the work for the data acquisition task
	void run (void);

public:
	// This constructor creates the motor task
	// REVIEW THIS
	task_motor (const char* p_name, unsigned portBASE_TYPE prio,
							  size_t stacked, emstream* serpt, Motor* MotorA, Motor* MotorB);
};

#endif // _TASK_MOTOR_H_
