//-------------------------------------------------------------------------------------

/** @brief 	 A controller class to read the sensors and control the motors
 *	@details The feedback from the IMU is converted to g's, compared to the
 *			 appropriate reference value, and the error undergoes proportianal
 *		   	 and integral algorithms to output an actuation signal for the
 *			 motor driver.
 *	@return	 A duty cycle expressed as an 8-bit integer for the range -100 to 100.
 */

class Balance
protected:
	float kp = 0;							// Proportional control gain
	float ki = 0;							// Integral control gain
	float kg = 0;                           // Gyroscope sensitivity gain

	float set_x = 0;					    // Setpoints for acceleration compare
	float set_y = 0;                        // in terms of mG
	float set_z = -1000;
	float set_x2 = 0;
	float set_y2 = 0;
	float set_z2 = 0;

	static float esum_x = 0;			    // Error sum for integral control in
	static float esum_y = 0;                // x and y directions.

	float x_accel;                          // Current converted accelerometer data
	float y_accel;
	//float z_accel;
	float x2_accel;
	float y2_accel;
	//float z2_accel;

	//float x_gyro;                          // Current converted angular velocity data
	//float y_gyro;
	//float z_gyro;
	//float x2_gyro;
	//float y2_gyro;
	//float z2_gyro;

	//float act_sig_x;                        // Actuation signal for motors x and y
	//float act_sig_y;

public:
    Balance (void);                         // Simple constructor

	void set_gains (void);					// Input proportional and integral gains
	void convert (void); 		// Converts IMU signals to mG
	void set_setpoint (void);    // Acquire appropriate setpoint value
	void controller (void);				    // Applies PI control to output actuation
											// signal
};
