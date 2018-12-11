//**************************************************************************************
/** @file task_imu.h
 *    This file contains the headers for an imu task which initializes two mma8451's and
 *    sets there actuation signal when run is called.
 */

//**************************************************************************************

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _TASK_IMU_H_
#define _TASK_IMU_H_

#include "taskbase.h"                       // This is a task; here's its parent
#include "shares.h"                         // Task queues and shared variables
#include "mma8452q.h"	                 // Class for a motor driver
#include "emstream.h"

//-------------------------------------------------------------------------------------
/** @brief   Task which reads acceleration data from an accelerometer
 *  @details This task reads the X, Y, and Z axis accelerations from an mma8452q
 *  accelerometer and updates a task share called accelerometer_data which is a buffer
 *  that holds the previous 5 acceleration data points
 */

class task_imu : public TaskBase
{
protected:
	/** @brief   The number of milliseconds per sample taken by this task.
	 */
	portTickType ms_per_sample;

	/** @brief   A pointer to an mma8452q accelerometer used by this task. Accelerometer
	 * must be initialized and active
	 */
    mma8452q* accelerometer;

    /** @brief Values used in calibration of IMU from its offset
	 */
    int16_t offsetA[3] = {-500, 300, 50};

	/** @brief Values used in calibrating IMU to mG
     */
	float calibrateA[3] = {16.425, 16.1, 16.15};

	/** @brief The run function for the task. No states in this run function
     */
	void run (void);

public:

	/** @brief The constructor for the task
     */
	task_imu (const char* p_name, unsigned portBASE_TYPE prio,
							  size_t stacked, emstream* serpt, mma8452q* accelerometerIn);
};

#endif // _TASK_IMU_H_
