//**************************************************************************************
/** \file task_controller.h
 *    This file contains headers for a motor controller. This task serves to output
 *    the appropriate duty cycle corresponding to the effort of the motor necessary to
 *    correct the position of the platform.
 *
 *  Revisions:
 *    \li 11-29-2018 LEW Original file
 *
 *  Accreditation:
 *    The structure of this file, organization and some content, was directly written by
 *    John Ridgely, 11-26-2012. I have been given his files and explicit permission to
 *    adapt said files for ME 507 project use. */
//**************************************************************************************

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _TASK_CONTROLLER_H_
#define _TASK_CONTROLLER_H_

#include "FreeRTOS.h"
#include "task.h"

#include "taskbase.h"                       // Base class for tasks
#include "shares.h"                         // Lists shares and queues between tasks



//-------------------------------------------------------------------------------------
/** @brief   Task which pulses an LED on and off as another responds to a button.
 *  @details This task controls some LED's on the STM32 Discovery board. The blue
 *           LED uses a PWM to glow brighter and dimmer, and the orange one can be
 *           lit up or turned off by using the blue button.
 */

class task_controller : public TaskBase
{
protected:
	Balance controller;

public:
	// The constructor sets up the task object and links it to a PolyDAQ 2 driver
	task_controller (const char* p_name, unsigned portBASE_TYPE prio, size_t stacked,
			   emstream* serpt);

	// The run method manages the LED as the program runs
	void run (void);

};


#endif  _TASK_CONTROLLER_H_
