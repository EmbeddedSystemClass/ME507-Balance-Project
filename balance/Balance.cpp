//*************************************************************************************
/** @file Balance.cpp
 *    This is a C++ file that implements a control loop for our ME 507 project. After
 *    hard-setting the PI control gains, this file should be capable of using the IMU
 *    sensor data to acquire an appropriate duty cycle for each motor.
 */
//*************************************************************************************

#include "Balance.h"						    // Include header file for project


//-------------------------------------------------------------------------------------
/** @brief   Creates a controller which holds the functions that determine actuation signal.
 *  @details This is a simple constructor. It does not require any setup.
 */

Balance::Balance (void)
{
}

//-------------------------------------------------------------------------------------
/** @brief   Set proportional and integral control gains.
 *  @details Allows the user to input the  necessary control gains.
 */

void Balance::set_gains (void)
{
	kp = 0.15;								    // Adjust proportional control
	ki = 0.1;								    // Adjust integral control
}


//-------------------------------------------------------------------------------------
/** @brief   Averages the previous 5 IMU values to avoid unwanted noise.
 *  @details Takes the accelBuf structure, creates a total of all available X, Y and Z
 *  values and finds the average
 *  @param buffer A structure that holds the previous 5 acceleration data points for X,
 *  Y, and Z axis of the IMU
 */

void Balance::convert (accelBuf buffer)
{
    int16_t xaccel_tot = 0;
    int16_t yaccel_tot = 0;
    int16_t zaccel_tot = 0;
    for(uint8_t i = 0; i < 5; i++)
    {
        xaccel_tot += buffer.accel_buffer[i].data[0];
        yaccel_tot += buffer.accel_buffer[i].data[1];
        zaccel_tot += buffer.accel_buffer[i].data[2];
    }
    x_accel = xaccel_tot/5;
    y_accel = yaccel_tot/5;
    z_accel = zaccel_tot/5;
}


//-------------------------------------------------------------------------------------
/** @brief   Dynamically adjusts the setpoint value.
 *  @details Uses acceleration information from both IMUs to output an appropriate
 *			 reference value to compare against the feedback. This function was left
 *			 simple for the controller used, but would ultimately be used to change
 *			 the set points of the controller.
 */

 // Currently implementing simple control, disregarding the magnitude of either IMU
 // and not caring about the IMU on the handle.
 void Balance::set_setpoint (void)
{
    set_x = 0;					        // Simple control for now
	set_y = 0;
	// Don't care about these readings for now.
	set_z = -1000;
	set_x2 = 0;                           // Currently disregarding handle IMU
	set_y2 = 0;
	set_z2 = 0;
}


//-------------------------------------------------------------------------------------
/** @brief   Applies PI control to output a duty cycle for each motor.
 *  @details Uses the error signal calculated from the setpoints for x and y to the
 *           current accelerations sensed in the x and y directions to calculate the
 *           appropriate actuation signal. The control gains convert the error in mG to
 *           voltage.
 */

 void Balance::control ()
 {
     // Integral sum of the error, delta T = .005s from the rate at which the controller is run
     esum_x += (set_x - x_accel) * .005;
     // PI control
     int16_t signalVal = (set_x - x_accel)*kp + esum_x*ki;
     motor_A_actuation_signal->put(signalVal);

     // Integral sum of the error, delta T = .005s from the rate at which the controller is run
     esum_y += (set_y - y_accel) * .01;
     // PI control
     signalVal = (set_y - y_accel)*kp + esum_y*ki;
     motor_B_actuation_signal->put(signalVal);
 }

