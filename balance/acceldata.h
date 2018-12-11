//*************************************************************************************
/** @file    acceldata.h
 *  @brief   Declarations for inter-task data communication types.
 *  @details This file contains structs to organize data for inter-task communication.
 */
//*************************************************************************************

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _ACCEL_DATA_H_
#define _ACCEL_DATA_H_

/** @brief   Structure to hold X, Y, and Z axis accelerations.
 *  @details data[0] = X-axis acceleration data[1] = Y-axis acceleration
 *  data[2] = Z-axis acceleration
 */
typedef struct
{
    /** @brief Array for acceleration axises
     */
    float data[3];
} accelData;

/** @brief   Structure to hold 5 accelData structs
 *  @details The current buffer size used is 5. A larger buffer would allow for the
 *  averaging of more data points from the accelerometer to filter noise
 */
typedef struct
{
    /** @brief Buffer of acceleration data
     */
    accelData accel_buffer[5];
}accelBuf;

#endif // _ACCEL_DATA_H_
