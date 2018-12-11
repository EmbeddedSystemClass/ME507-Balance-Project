//**************************************************************************************
/** \file task_controller.h
 *    This file contains headers for a motor controller. This task serves to output
 *    the appropriate duty cycle corresponding to the effort of the motor necessary to
 *    correct the position of the platform.
 */
//**************************************************************************************

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _TASK_CONTROLLER_H_
#define _TASK_CONTROLLER_H_

#include "FreeRTOS.h"
#include "task.h"
#include "Balance.h"

#include "taskbase.h"                       // Base class for tasks
#include "shares.h"                         // Lists shares and queues between tasks



//-------------------------------------------------------------------------------------
/** @brief   Task which uses accelerometer data to calculate motor actuation signals
 *  @details This task uses the task share data made available from the IMU task to set
 *  the task shares for motor A and B actuation signals.
 */

class task_controller : public TaskBase
{
protected:
	/** @brief The controller that is used by the task to implement the control algorithm
	 */
	Balance *controller;
public:
	/** @brief The constructor sets up the task object
	 */
	task_controller (const char* p_name, unsigned portBASE_TYPE prio, size_t stacked,
		emstream* serpt, Balance* controller);

	/** @brief The run method call the functions of the controller in a loop
	 */
	void run (void);
};
#endif //_TASK_CONTROLLER_H_