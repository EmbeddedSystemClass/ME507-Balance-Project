//*************************************************************************************
/** @file    main.cpp
 *  @brief   The main file for a ME507 balancing device.
 *  @details It
 *           contains the declarations to globally accessible pointers to the shared
 *           data items and queues which are used to exchange information between
 *           tasks; code which creates the pointers to the shares and queues; and the
 *           function @c main(), which is the first function to run when the program 
 *           is started up. Inside @c main(), code does initial setup, creates the 
 *           task objects which are used in the program, and starts the RTOS scheduler.
 *
 *  Revisions:
 *    \li 08-26-2014 JRR Original file, based on earlier PolyDAQ2 project files
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

#include <math.h>                               ////////////////////////////////////////////
// extern "C" 
// {
// 	int __errno;                           // WTF
// }

#include "FreeRTOS.h"
#include "task.h"

#include "rs232.h"                          // Header for serial port class
#include "taskbase.h"                       // The base class for all tasks
#include "taskqueue.h"                      // Queues transmit data between tasks
#include "motorDriver.h"
#include "task_motor.h"                      // Application configuration header
#include "task_controller.h"
#include "task_imu.h"

//-------------------------------------------------------------------------------------
// The pointers in the following section are for shares and queues that transfer data,
// commands, and other information between tasks. 

/** @brief   Pointer to a queue for text to be displayed by the UI.
 *  @details This queue can be used by all the tasks to print messages without having
 *           to wait for the comparatively slow serial port.
 */
//TextQueue* p_main_text_queue;



/** @brief   Pointer to a share of the motor 1 actuation signal.
 *  @details This shared pointer contains the memory address of the motor1
 *           actuation signal.
 */
TaskShare<int8_t>* motor_A_actuation_signal;

/** @brief   Pointer to a share of the motor 2 actuation signal.
 *  @details This shared pointer contains the memory address of the motor1
 *           actuation signal.
 */
TaskShare<int8_t>* motor_B_actuation_signal;

/** @brief   Pointer to a share of the motor 2 actuation signal.
 *  @details This shared pointer contains the memory address of the motor1
 *           actuation signal.
 */
TaskShare <accel_buffer>* accelerometer_data;




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
	*usart_2 << endl << clrscr << "FreeRTOS Test Program on STM32" << endl;


	// Initialize the I/O port pin ENa of motor 1
	/*GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // DONT NEED ALTERNATE FUNC FOR THIS?
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // push pull
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
	GPIO_Init (GPIOB, &GPIO_InitStruct);

	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);*/

	// Create I2C bus for the IMUs
	//p_i2c1 = new i2c_master (GPIOB, 6, 7, NULL); //i2c1
    //p_i2c2 = new i2c_master (GPIOB, 8, 9, NULL); //i2c1 as well??

	//------------------------------- Queues and Shares -------------------------------

	/*  This queue prints out debugging and other messages.
	 */
	motor_A_actuation_signal = new TaskShare<int8_t> ("MotorA act");
        
        motor_B_actuation_signal = new TaskShare<int8_t> ("MotorB act");


        accelerometer_data = new TaskShare <accelBuf> ("accel data");

	/*  This queue carries commands from the user interface task to the data 
	 *  acquisition task.
	 */


	//--------------------------------- Device Drivers --------------------------------

	// creates the two IMUs which are both on i2c1
        i2c_master* i2c1 = new i2c_master (GPIOB, 8, 9, NULL); //i2c1  //B8 B9
       
        
        // scan for I2C
        i2c1->scan(usart_2);
        mma8452q* accel1 = new mma8452q(i2c1, 0x3A, usart_2);
        accel1->initialize();
        accel1->active();
	
	//PC4
    // Enable the clock to A pins
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);

    // Initialize the I/O port pin ENa for motor 1
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // DONT NEED ALTERNATE FUNC FOR THIS?
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // push pull
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
    GPIO_Init (GPIOA, &GPIO_InitStruct);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
   
    // enable clock to C pins
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOC, ENABLE);

    // Initialize the I/O port pin ENb for motor 2
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // DONT NEED ALTERNATE FUNC FOR THIS?
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // push pull
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
    GPIO_Init (GPIOC, &GPIO_InitStruct);
    
    GPIO_SetBits(GPIOC, GPIO_Pin_1);

         // motor 1 stuff 
          hw_pwm* motor_A_In1 = new hw_pwm (3, 20000, 100);// timers and pins
          (*motor_A_In1).activate_pin(GPIOB, 5, TIM_OC2Init, TIM_OC2PreloadConfig); // change pins
    
          hw_pwm* motor_B_In1 = new hw_pwm (3, 20000, 100);// timers and pins
          (*motor_B_In1).activate_pin(GPIOB, 4, TIM_OC1Init, TIM_OC1PreloadConfig); // change pins
    
         // motor 2 stuff
         hw_pwm* motor_A_In2 = new hw_pwm (5, 20000, 100);// timers and pins
         (*motor_A_In2).activate_pin(GPIOA, 0, TIM_OC1Init, TIM_OC1PreloadConfig); // change pins
    
         hw_pwm* motor_B_In2 = new hw_pwm (5, 20000, 100);// timers and pins
        (*motor_B_In2).activate_pin(GPIOA, 1, TIM_OC2Init, TIM_OC2PreloadConfig); // change pins 
        
        Motor* motor1 = motor(motor_A_In1, motor_B_In1, 2, 1);
        Motor* motor2 = motor(motor_A_In2, motor_B_IN2, 1, 2);





	//*********************************************************************************

	//------------------------------------- Tasks -------------------------------------

	// create a motor task for motor 1
	new task_motor ("Motor_1_task", 1, 240, NULL, motor1, motor2);
        new task_imu ("IMU task", 2, 400, NULL, accel1,accel1);
        new task_controller ("controller task", 3, 800, NULL);

// 	// This task prints characters from the main print queue
// 	new task_print ("PrintQ", 1, 240, usart_2);


	//Start the FreeRTOS scheduler
	vTaskStartScheduler ();

	return (0);
}
