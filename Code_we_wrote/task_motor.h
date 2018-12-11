//**************************************************************************************
/** @file task_motor.h
 *    This file contains the headers for a motor task which takes a motorDriver object
 *    and sets its actuation signal according to a task share
 *
 */

//**************************************************************************************

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _TASK_MOTOR_H_
#define _TASK_MOTOR_H_

#include "taskbase.h"                       // This is a task; here's its parent
#include "shares.h"                         // Task queues and shared variables
#include "motorDriver.h"	                 // Class for a motor driver

//-------------------------------------------------------------------------------------
/** @brief   Task which takes in an initialized motor and sets the actuation signal
 *  @details This task acquires actuation signal from a control task and sets the
 *  actuation to each motor
 */

class task_motor : public TaskBase
{
protected:
	/** @brief   The number of milliseconds per sample taken by this task.
	 */
	portTickType ms_per_sample;

	/** @brief   A pointer to motor drivers used by this task.
	 */
    Motor* motor;

    /** @brief   A value to distinguish motors for the task shares used.
     * 0 = motor_A_actuationSignal
     * 1 = motor_B_actuationSignal
     */
    uint8_t motor_val;

	/** @brief  The function which does all the work for the motor task
	 */
	void run (void);

public:
    /** @brief This constructor creates the motor task
     */
    task_motor (const char* p_name, unsigned portBASE_TYPE prio,
        size_t stacked, emstream* serpt, Motor* MotorIn, uint8_t motor_num);
};

#endif // _TASK_MOTOR_H_
