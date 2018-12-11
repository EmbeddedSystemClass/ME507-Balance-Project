//*************************************************************************************
/** @file Balance.cpp
 *    This is a C++ file that implements a control loop for our ME 507 project. After
 *    hard-setting the PI control gains, this file should be capable of using the IMU
 *    sensor data to acquire an appropriate duty cycle for each motor.
 *
 *  @b Revisions:
 *    11-27-2018 LEW Original file
 *
 *  @b Usage:
 *    This file is intended to be compiled and programmed to our PCB, containing an
 *    STM32F411 MCU.
 *
 *  @b Accreditation:
 *    The structure of this file, organization and some content, was directly written by
 *    John Ridgely, 11-26-2012. I have been given his files and explicit permission to
 *    adapt said files for ME 507 project use. */
//*************************************************************************************

#include "Balance.h"						    // Include header file for project


//-------------------------------------------------------------------------------------
/** @brief   Create an empty array for the future accelerometer data.
 *  @details This is a simple constructor. It does not require any setup.
 */

Balance::Balance (void)
{
}

//-------------------------------------------------------------------------------------
/** @brief   Set proportional and integral control gains.
 *  @details Allows the user to input the necessary control gains.
 *  @return  float values for kp and ki.
 */

void Balance::set_gains (void)
{
	kp = 0.15;								    // Adjust proportional control
	ki = 0.30;								    // Adjust integral control
}


//-------------------------------------------------------------------------------------
/** @brief   Converts IMU readings to discernible units of milliG's.
 *  @details Takes the input value and divides by a value obtained from calibration to
 *			 acquire the acceleration in terms of Earth's gravity.
 */

void Balance::convert (void);        // call with array of size 6
// p_AccelRaw[0] = x_accel_raw, p_AccelRaw[1] = y_accel_raw, ... p_AccelRaw[5] = z2_accel_raw
// p_GyroRaw[0] = x_accel_raw, p_GyroRaw[1] = y_accel_raw, ... p_GyroRaw[5] = z2_accel_raw

{
    int16_t x_accel_tot = 0;
	int16_t y_accel_tot = 0;
	int16_t z_accel_tot = 0;
	for(uint8_t i = 0; i < 10; i++){
	    xaccel_tot = accelerometer_data->accel_buffer[i].accelerometer_A_data[0];
        yaccel_tot = accelerometer_data->accel_buffer[i].accelerometer_A_data[1];
        xaccel_tot = accelerometer_data->accel_buffer[i].accelerometer_A_data[2];
	}
	xaccel = xaccel_tot/10;
	yaccel = yaccel_tot/10;
	zaccel = zaccel_tot/10;
//	x2_accel = p_AccelRaw[3]/12345// LINEAR ACCELERATION CALIBRATION VALUE
//	y2_accel = p_AccelRaw[4]/12345// LINEAR ACCELERATION CALIBRATION VALUE
//	z2_accel = p_AccelRaw[5]/12345// LINEAR ACCELERATION CALIBRATION VALUE
//
//	x_gyro = p_GyroRaw[0]/56789// ANGULAR VELOCITY CALIBRATION VALUE
//	y_gyro = p_GyroRaw[1]/56789// ANGULAR VELOCITY CALIBRATION VALUE
//	z_gyro = p_GyroRaw[2]/56789// ANGULAR VELOCITY CALIBRATION VALUE
//	x2_gyro = p_GyroRaw[3]/56789// ANGULAR VELOCITY CALIBRATION VALUE
//	y2_gyro = p_GyroRaw[4]/56789// ANGULAR VELOCITY CALIBRATION VALUE
//	z2_gyro = p_GyroRaw[5]/56789// ANGULAR VELOCITY CALIBRATION VALUE
}


//-------------------------------------------------------------------------------------
/** @brief   Dynamically adjusts the setpoint value.
 *  @details Uses acceleration information from both IMUs to output an appropriate
 *			 reference value to compare against the feedback.
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
 *  @details Uses the error signal given from comparing the setpoint for x and y to the
 *           current accelerations sensed in the x and y directions to calculate the
 *           appropriate actuation signal. The control gains convert the error in mG to
 *           voltage.
 */

 void Balance::control (void)
 {
    esum_x += set_x - x_accel;                      // Integral sum of the error
    *motor_A_actuation_signal = (set_x - x_accel)*kp + esum_x*ki;

    esum_y += set_y - y_accel;
    *motor_A_actuation_signal = (set_y - y_accel)*kp + esum_y*ki;

 }


 //-------------------------------------------------------------------------------------
/** @brief   Saturates the actuation signal and gives it as a percent duty cycle.
 *  @details The control gains are adjusted such that an actuation signal of 100/-100 is
 *           the maximum effort of the motor. If the signal is greater than that, it is
 *           this tasks' job to saturate the signal back to 100/-100.


 int8_t Balance::saturate (void)
{
    if act_sig_x > 100
    {
        act_sig_x = 100;
    }
    if act_sig_x < -100
    {
        act_sig_x = -100;
    }
    if act_sig_y > 100
    {
        act_sig_y = 100;
    }
    if act_sig_y < 100
    {
        act_sig_y = -100;
    }
    int8_t duty_x = act_sig_x;
    int8_t duty_y = act_sig_y;
    return(duty_x,duty_y);
}
*/