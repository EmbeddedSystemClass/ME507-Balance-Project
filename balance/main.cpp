//*************************************************************************************
/** @file    main.cpp
 *  @brief   The main file for a ME507 balancing device.
 *  @details Contains the declarations to globally accessible pointers to the shared
 *           data items and queues which are used to exchange information between
 *           tasks; code which creates the pointers to the shares and queues; and the
 *           function @c main(), which is the first function to run when the program 
 *           is started up. Inside @c main(), code does initial setup, creates the 
 *           task objects which are used in the program, and starts the RTOS scheduler.
 *
 *  License:
 *    This file is copyright 2014 by JR Ridgely and released under the Lesser GNU 
 *    Public License, version 2. It intended for educational use only, but its use
 *    is not limited thereto. */
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUEN-
 *    TIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 *    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 *    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 *    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
//*************************************************************************************

#include <math.h>
#include "FreeRTOS.h"                       // Header for scheduler
#include "task.h"                           // Header for task

#include "rs232.h"                          // Header for serial port class
#include "taskbase.h"                       // The base class for all tasks
#include "taskqueue.h"                      // Queues transmit data between tasks
#include "motorDriver.h"                    // Header for motor class
#include "task_motor.h"                     // Header for motor task
#include "task_controller.h"                // Header for controller task
#include "task_imu.h"                       // Header for sensor task
#include "Balance.h"                        // Header for controller object
#include "acceldata.h"                      // Header for acceleration data struct

//-------------------------------------------------------------------------------------
// The pointers in the following section are for shares and queues that transfer data,
// commands, and other information between tasks. 

/** @brief   Pointer to a queue for text to be displayed by the UI.
 *  @details This queue can be used by all the tasks to print messages without having
 *           to wait for the comparatively slow serial port.
 */
//TextQueue* p_main_text_queue;

/** @brief   Pointer to a share of the motor A actuation signal.
 *  @details This shared pointer contains the memory address of the motorA actuation
 *           signal.
 */
TaskShare<int16_t>* motor_A_actuation_signal;

/** @brief   Pointer to a share of the motor B actuation signal.
 *  @details This shared pointer contains the memory address of the motorB actuation
 *           signal.
 */
TaskShare<int16_t>* motor_B_actuation_signal;

/** @brief   Pointer to a share for accelerometer A data.
 *  @details Buffer size 10 of X,Y, and Z axis of accelerometer A
 */
TaskShare <accelBuf>* accelerometer_A_data;

/** @brief   Pointer to a share for accelerometer B data.
 *  @details Buffer size 10 of X,Y, and Z axis of accelerometer B
 */
TaskShare <accelBuf>* accelerometer_B_data;


//-------------------------------------------------------------------------------------
/** @brief   This function runs when the application is started up.
 *  @details The @c main() function instantiates shared variables and queues, sets up 
 *           serial ports and other drivers (if needed), creates the task objects 
 *           which will run @a ad @a infinitum, and starts the RTOS scheduler. 
 *  @return  Although the scheduler never exits and this function never returns, it's
 *           C/C++ tradition for @c main() to return an integer, so we (don't) return
 *           zero; if the return statement is missing the compiler issues a warning.
 */

int main (void)
{
	// Create the serial port which will be used for programing and communicating with PC
	RS232* usart_2 = new RS232 (USART2, 115200);
	*usart_2 << endl << clrscr << "FreeRTOS Program on STM32" << endl;

	//------------------------------- Queues and Shares -------------------------------

	/*  This share holds the motor A actuation signal.
	 */
	motor_A_actuation_signal = new TaskShare<int16_t> ("MotorA act");

    /*  This share holds the motor B actuation signal.
     */
	motor_B_actuation_signal = new TaskShare<int16_t> ("MotorB act");

    /*  Buffer size 10 of X,Y, and Z axis of accelerometer A
     */
    accelerometer_A_data = new TaskShare <accelBuf> ("Accel A data");

   /*  Buffer size 10 of X,Y, and Z axis of accelerometer B
    */
    accelerometer_B_data = new TaskShare <accelBuf> ("Accel B data");

	//--------------------------------- Device Drivers --------------------------------

	// Creates the i2c used for the accelerometer
    // Pins B8 B9
	i2c_master* i2c1 = new i2c_master (GPIOB, 8, 9, NULL);

	// Creates and initializes accelerometer
	mma8452q* accel1 = new mma8452q(i2c1, 0x3A, usart_2);
	accel1->initialize();
	accel1->active();

	// Print statement to serial port to show IMU created if connected correctly
	*usart_2 << endl << "IMU activated" << endl;

    // Motor 1 pwm pins configuration
    // pin B5, TIM3 CH2
    hw_pwm* motor_A_In1 = new hw_pwm (3, 20000, 100);
    (*motor_A_In1).activate_pin(GPIOB, 5, TIM_OC2Init, TIM_OC2PreloadConfig);
    // pin B4, TIM3 CH1
    hw_pwm* motor_A_In2 = new hw_pwm (3, 20000, 100);
    (*motor_A_In2).activate_pin(GPIOB, 4, TIM_OC1Init, TIM_OC1PreloadConfig);
    
    // Motor 2 pwm pins configuration
    // pin A0, TIM5 CH1
    hw_pwm* motor_B_In1 = new hw_pwm (5, 20000, 100);// timers and pins
    (*motor_B_In1).activate_pin(GPIOA, 0, TIM_OC1Init, TIM_OC1PreloadConfig); // change pins
    // pin A1, TIM5 CH2
    hw_pwm* motor_B_In2 = new hw_pwm (5, 20000, 100);// timers and pins
    (*motor_B_In2).activate_pin(GPIOA, 1, TIM_OC2Init, TIM_OC2PreloadConfig); // change pins

    // Motor drivers
    //EN pin C1
    Motor* motorA = new Motor(motor_A_In1, motor_A_In2, 2, 1, GPIO_Pin_1,
            GPIOC, RCC_AHB1Periph_GPIOC);

    //EN pin A10
    Motor* motorB = new Motor(motor_B_In1, motor_B_In2, 1, 2, GPIO_Pin_10,
            GPIOA, RCC_AHB1Periph_GPIOA);

    //Controller object passed to controller task
    Balance* controller = new Balance();

	//*********************************************************************************

	//------------------------------------- Tasks -------------------------------------

	// This task controls actuation of motorA
	new task_motor ("Motor_A_task", 1, 240, NULL, motorA);

    // This task controls actuation of motorA
    new task_motor ("Motor_B_task", 1, 240, NULL, motorB);

    // This task reads accelerations in X, Y, and Z axis. Only X and Y axis used for this controller
	new task_imu ("IMU 1 task", 2, 400, NULL, accel1);

	// This task averages acceleration data and uses the controller to determine motor actuation signals
	new task_controller ("Controller task", 3, 800, usart_2, controller);

	// Print statement to serial port showing the start of tasks running
    *usart_2 << endl << clrscr << "Scheduler about to run" << endl;

    //Start the FreeRTOS scheduler
	vTaskStartScheduler ();

	return (0);
}
