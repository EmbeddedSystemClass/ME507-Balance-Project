//**************************************************************************************
/** \file task_leds.h
 *    This file contains the headers for an LED blinking task for an STM32 Discovery
 *    board. This task is mostly used to provide assurance that the unit is working.
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
#ifndef _TASK_LEDS_H_
#define _TASK_LEDS_H_

#include "FreeRTOS.h"
#include "task.h"

#include "rs232.h"
#include "polydaq2.h"                       // Drivers for stuff on the PolyDAQ2 board

#include "taskbase.h"                       // Base class for tasks
#include "shares.h"                         // Lists shares and queues between tasks



//-------------------------------------------------------------------------------------
/** @brief   Task which pulses an LED on and off as another responds to a button.
 *  @details This task controls some LED's on the STM32 Discovery board. The blue
 *           LED uses a PWM to glow brighter and dimmer, and the orange one can be
 *           lit up or turned off by using the blue button.
 */

class task_sensor : public TaskBase
{
protected:
    // Creates task_sensor as an object of the class "Balance".
	Balance task_sensor

public:
	// The constructor sets up the task object and links it to a PolyDAQ 2 driver
	task_sensor (const char* p_name, unsigned portBASE_TYPE prio, size_t stacked,
			   emstream* serpt, polydaq2* p_polydaq2);

	// The run method manages the LED as the program runs
	void run (void);

	/// @brief   Turn off the LED, making sure it's not in an automatic mode.
	void off (void)
	{
		manual_mode = true;
		my_poly->set_SD_card_LED_brightness (0);
	}

	/// @brief   Turn on the LED, making sure it's not in an automatic mode.
	void on (void)
	{
		manual_mode = true;
		my_poly->set_SD_card_LED_brightness (SD_CARD_LED_MAX_PWM);
	}

	/// @brief   Put the LED in automatic "heartbeat" display mode.
	void heartbeat (void)
	{
		manual_mode = false;
	}
};


#endif // _TASK_LEDS_H_
