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
//#include "task_motor.h"                      // Application configuration header

    #include "tm_stm32f4_delay.h"

//#include "IMU.h"
#include "mma8452q.h"

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
//TaskShare<uint8_t>* motor_1_actuation_signal;

/** @brief   Pointer to a share of the motor 2 actuation signal.
 *  @details This shared pointer contains the memory address of the motor1
 *           actuation signal.
 */
//TaskShare<uint8_t>* motor_2_actuation_signal;




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
        
        
        
        
        i2c_master* i2c1 = new i2c_master (GPIOB, 8, 9, NULL); //i2c1  //B8 B9
       
        
        // scan for I2C
        i2c1->scan(usart_2);
        mma8452q* accel1 = new mma8452q(i2c1, 0x3A, usart_2);
        accel1->initialize();
        accel1->active();
        
         /*
        	// creates the two IMUs which are both on i2c1
	IMU* imu1 = new IMU(i2c1,0xD6, usart_2);  // Unsure about address sent here for LSM6DSL
	//IMU imu2 = new IMU(i2c2,0x6A, NULL);	// I2C addresses are the same? look at page 38 of datasheet
        
        imu1->initialize();    
	imu1->active ();
        
         uint8_t regVal = i2c1->read (0xD6, 0x0F);
        *usart_2 << PMS ("Addr x0F") << ": " <<regVal << "  ";
       // regVal = i2c1->read (0xD6, 0x0F);
        //*usart_2 << PMS ("Addr x0F") << regVal << "  ";
         //regVal = i2c1->read (0xD6, 0x0D);
        //*usart_2 << PMS ("Addr x0D") << regVal << "  ";
        //regVal = i2c1->read (0xD6, 0x03);
        //*usart_2 << PMS ("Addr x03") << regVal << "  ";
         //regVal = i2c1->read (0xD6, 0x04);
      // *usart_2 << PMS ("Addr x04") << regVal << "  ";
 
     //  for(;;)
      //{
        regVal = i2c1->read (0xD6, 0x10);
        *usart_2 << PMS ("Addr x10") << ": " <<regVal << "  " << endl;
        regVal = i2c1->read (0xD6, 0x11);
        *usart_2 << PMS ("Addr x11") << ": " <<regVal << "  " << endl;
        regVal = i2c1->read (0xD6, 0x0A);
        *usart_2 << PMS ("Addr x0A") << ": " <<regVal << "  " << endl ;
        regVal = i2c1->read (0xD6, 0x15);
        *usart_2 << PMS ("Addr x15") << ": " <<regVal << "  " << endl ;
        
        // read acc or gyro indiviudally
  
        regVal = i2c1->read (0xD6, 0x22);
        *usart_2 << PMS ("Addr x22") << ": " <<regVal << "  "  ;
        regVal = i2c1->read (0xD6, 0x23);
        *usart_2 << PMS ("Addr x23") << ": " <<regVal << "  " ;
        regVal = i2c1->read (0xD6, 0x24);
        *usart_2 << PMS ("Addr x24") << ": " <<regVal << "  " ;
        regVal = i2c1->read (0xD6, 0x25);
        *usart_2 << PMS ("Addr x25") << ": " <<regVal << "  " ;
        regVal = i2c1->read (0xD6, 0x26);
        *usart_2 << PMS ("Addr x26") << ": " <<regVal << "  " ;
        regVal = i2c1->read (0xD6, 0x27);
        *usart_2 << PMS ("Addr x27") << ": " <<regVal << "  " << endl;
        
        
        */

	*usart_2 << PMS ("X Axis: ") << accel1->get_one_axis (0) << "   ";
        *usart_2 << PMS ("Y Axis: ") << accel1->get_one_axis (1) << "   ";
        *usart_2 << PMS ("Z Axis: ") << accel1->get_one_axis (2) << endl;

    
	/*GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // DONT NEED ALTERNATE FUNC FOR THIS?
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // push pull
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
	GPIO_Init (GPIOB, &GPIO_InitStruct);

	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
	


	// Create I2C bus for the IMUs
	
    //p_i2c2 = new i2c_master (GPIOB, 8, 9, NULL); //i2c1 as well??

	//------------------------------- Queues and Shares -------------------------------

	/*  This queue prints out debugging and other messages.
	 */
	//p_main_text_queue = new TextQueue (400, "DBG Text", 0, 10);

	//motor_1_actuation_signal = new



	/*  This queue carries commands from the user interface task to the data 
	 *  acquisition task.
	 */


	//--------------------------------- Device Drivers --------------------------------

	// creates the two IMUs which are both on i2c1
	//IMU imu1 = new IMU(i2c1,0x6A, NULL);
	//IMU imu2 = new IMU(i2c2,0x6A, NULL);	// I2C addresses are the same?


	//CHANGE ALL MOTOR PIN INPUTS
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
    
    
 /*
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);

    // Initialize the I/O port pin ENa of motor 1
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // DONT NEED ALTERNATE FUNC FOR THIS?
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // push pull
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
    GPIO_Init (GPIOA, &GPIO_InitStruct);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_10);   
 */   
  /*  
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);

    // Initialize the I/O port pin ENa of motor 1
    GPIO_InitTypeDef GPIO_InitStruct2;
    GPIO_InitStruct2.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_OUT; // DONT NEED ALTERNATE FUNC FOR THIS?
    GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
    GPIO_InitStruct2.GPIO_OType = GPIO_OType_PP; // push pull
    GPIO_InitStruct2.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
    GPIO_Init (GPIOA, &GPIO_InitStruct2);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);
    // Enable the clock to the GPIO port
    //RCC_AHB1PeriphClockCmd (_CLOCK, ENABLE);*/
/*
    // Initialize the I/O port pin ENa of motor 1
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = _PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // DONT NEED ALTERNATE FUNC FOR THIS?
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Speed okay?
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // push pull
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // pull up instead of down/none
    GPIO_Init (_PORT, &GPIO_InitStruct);

*/

    // NUCLEO
    //D3 = pB3 TIM2_chan2
    //D6 = pB10 TIM2_chan3

    // PA6 = TIM3_ch1
    // PA7 = TIM3_ch2
    
 /*   hw_pwm* motor_A_In1 = new hw_pwm (3, 10000, 100);// timers and pins
    (*motor_A_In1).activate_pin(GPIOA, 6, TIM_OC1Init, TIM_OC1PreloadConfig); // change pins

    hw_pwm* motor_A_In2 = new hw_pwm (3, 10000, 100);
    (*motor_A_In2).activate_pin(GPIOA, 7, TIM_OC2Init, TIM_OC2PreloadConfig);

    Motor* motorA = new Motor(motor_A_In1, motor_A_In2, 1, 2);

    motorA->setActuation(-60);
*/
 
      float Xraw=accel1->get_one_axis (0);   
     float Yraw=accel1->get_one_axis (1);
     float Zraw=accel1->get_one_axis (2);
     
     float Xval= Xraw/16000;
     float Yval= Yraw/16000;
     float Zval= Zraw/16000;
     
     float xduty_set_point = 70*Xval;
     float yduty_set_point = 70*Yval;
     
     uint16_t counter12 = 0;
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
 for (;;)
 {
     counter12 += 1;
     
     Xraw=accel1->get_one_axis (0);   
     Yraw=accel1->get_one_axis (1);
     Zraw=accel1->get_one_axis (2);
     
     Xval= Xraw/16000;
     Yval= Yraw/16000;
     Zval= Zraw/16000;
     
     xduty_set_point = 90*Xval;
     yduty_set_point = 90*Yval;
     

    if (Xval > 0)
    {
     motor_A_In1->set_duty_cycle(2, xduty_set_point);
    }
    else{
     motor_B_In1->set_duty_cycle(1, -xduty_set_point);
    }
    
 
    if (Yval>0)
    {
     motor_A_In2->set_duty_cycle(1, yduty_set_point);
    }
    else
    {
     motor_B_In2->set_duty_cycle(2, -yduty_set_point);      
    }
    
    	*usart_2 << PMS ("X dutyset: ") << xduty_set_point << "   ";
        *usart_2 << PMS ("Y dutyset: ") << yduty_set_point << "   " ; 
        *usart_2 << PMS ("count: ") << counter12 << "   " << endl;
     
 }
/* 
    //D4=PB5(TIM3 CH2) & D5=PB4(TIM3 Ch1)
    hw_pwm* motor_A_In1 = new hw_pwm (3, 20000, 100);// timers and pins
    (*motor_A_In1).activate_pin(GPIOB, 5, TIM_OC2Init, TIM_OC2PreloadConfig); // change pins
    
    hw_pwm* motor_B_In1 = new hw_pwm (3, 20000, 100);// timers and pins
    (*motor_B_In1).activate_pin(GPIOB, 4, TIM_OC1Init, TIM_OC1PreloadConfig); // change pins  
        
   // Motor* motorA = new Motor(motor_A_In1, motor_B_In1, 2, 1);
   // motorA->setActuation(60);
    
     motor_A_In1->set_duty_cycle(2, 50);
     motor_B_In1->set_duty_cycle(1, 5);   
  
    //second motor (PA0 & PA1)
    hw_pwm* motor_A_In2 = new hw_pwm (5, 20000, 100);// timers and pins
    (*motor_A_In2).activate_pin(GPIOA, 0, TIM_OC1Init, TIM_OC1PreloadConfig); // change pins
    
    hw_pwm* motor_B_In2 = new hw_pwm (5, 20000, 100);// timers and pins
    (*motor_B_In2).activate_pin(GPIOA, 1, TIM_OC2Init, TIM_OC2PreloadConfig); // change pins  
        
    
   // Motor* motorB = new Motor(motor_A_In2, motor_B_In2, 3, 2);
   // motorB->setActuation(-60);
    
     motor_A_In2->set_duty_cycle(1, 25);
     motor_B_In2->set_duty_cycle(2, 90);    
   
*/

    //hw_pwm* motor_A_In2 = new hw_pwm (2, 10000, 100);
    //(*motor_A_In2).activate_pin(GPIOB, 10, TIM_OC3Init, TIM_OC3PreloadConfig);

    //Motor* motorA = new Motor(motor_A_In1, motor_A_In2, 2, 3);

    //motorA->setActuation(60);
    //hw_pwm motor_B_In_1 = new hw_pwm (timer_number, frequency, resolution);
    //motor_A_In_1.activate_pin(GPIOB, 12, tim_init_fn, tim_preload_fn);

    //hw_pwm motor_B_In2 = new hw_pwm (uint8_t timer_number, uint32_t frequency, uint16_t resolution);
    //motor_A_In2.activate_pin(GPIOB, 13, TIM_OC4Init, TIM_OC4PreloadConfig);




	//*********************************************************************************

	//------------------------------------- Tasks -------------------------------------

	// create a motor task for motor 1
	//new task_motor ("Motor_1_task", 1, 240, usart_2);


// 	// This task prints characters from the main print queue
// 	new task_print ("PrintQ", 1, 240, usart_2);


	// Start the FreeRTOS scheduler
	//vTaskStartScheduler ();

	return (0);
}
