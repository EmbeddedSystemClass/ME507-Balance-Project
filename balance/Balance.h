/** @brief 	 A controller class to read the sensors and control the motors
 *	@details The feedback from the IMU is converted to g's, compared to the
 *			 appropriate reference value, and the error undergoes proportianal
 *		   	 and integral algorithms to output an actuation signal for the
 *			 motor driver.
 *	@return	 A duty cycle expressed as an 8-bit integer for the range -100 to 100.
 */
#ifndef _BALANCE_H
#define _BALANCE_H

#include <stdint.h>
#include "emstream.h"
#include "shares.h"
#include "acceldata.h"

//-------------------------------------------------------------------------------------
class Balance
{
protected:
    
    /** @brief Proportional control gain
     */
    float kp = 0;
    
    /** @brief Integral control gain
     */
    float ki = 0;
    
    /** @brief Gyroscope sensitivity gain
     */
    float kg = 0;                          

    /** @brief Setpoints for acceleration compare in terms of mG
     */
    float set_x = 0;					    
    
    /** @brief Setpoints for acceleration compare in terms of mG
     */
    float set_y = 0;

    /** @brief Error sum for integral control in x direction
     */
    float esum_x = 0;
    
    /** @brief Error sum for integral control in y direction
     */
    float esum_y = 0;

    /** @brief Averaged acceleration in the x direction
     */
    float x_accel;

    /** @brief Averaged acceleration in the y direction
     */
    float y_accel;
    
    /** @brief Averaged acceleration in the z direction
     */
    float z_accel;



public:
    Balance ();                             // Simple constructor
    void set_gains (void);					 // Input proportional and integral gains
    void convert (accelBuf buffer); 		 // Converts IMU signals to mG
    void set_setpoint (void);               // Acquire appropriate setpoint value
    void control ();           			     // Applies PI control to output actuation signal
};
#endif