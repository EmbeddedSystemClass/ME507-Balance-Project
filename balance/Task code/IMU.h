//*************************************************************************************
/** \file IMU.h
 *    This file contains a driver class for a Freescale LSM6DSL acceleromter.
 *    The dumbhead accelerometer can have I2C addresses, /////// UNKNOWN 0x1C or 0x1D.
 *
 *  License:
 *    This file is copyright 2013 by JR Ridgely and released under the Lesser GNU
 *    Public License, version 2. It is intended for educational use only, but its use
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
#ifndef IMU_H
#define IMU_H

#include <stdlib.h>                         // Standard C/C++ library stuff
#include "i2c_bitbang.h"                    // Header for I2C (AKA TWI) bus driver
#include "emstream.h"                       // Header for base serial devices#ifndef IMU_H



/** This is the I2C bus address of the sensor as used for writing commands and data to
 *  the sensor. Sensors are shipped with a default 7-bit address of 0x1C. This address
 *  doesn't include the read/write bit, so in the terminology used in the AVR data
 *  sheets, we form a \c SLA+W number by adding the seventh bit (0) as the LSB.
 */
const uint8_t IMU_WRITE_ADDRESS = (0x1D << 1);


/// @brief   The register address of @c CTRL1_XL_REG within the LSM6DSL accelerometer.
#define CTRL1_XL_REG1       0x2A  // Or with 01000000

/** @brief   Values of the two-bit number used to set the LSM6DSL's full scale range.
 */
/*enum LSM6DSLq_range_t
{
    MMA_RANGE_2g = 0x00,   ///< @brief A code to set the range of the MMA8452Q to +/-2g
    MMA_RANGE_4g = 0x01,   ///< @brief A code to set the range of the MMA8452Q to +/-4g
    MMA_RANGE_8g = 0x02    ///< @brief A code to set the range of the MMA8452Q to +/-8g
};*/


//-------------------------------------------------------------------------------------
/** @brief   Driver for a LSM6DSL accelerometer on an I2C bus.
 *  @details This class is a driver for a Freescale LSM6DSL accelerometer. It's very
 *           basic and it is set up only to grab acceleration data from the X, Y,
 *           and Z channels using the I2C bus.
 *
 *  @section use LSM6DSL Usage
 *           To use this driver, one need only create the driver object, put the
 *           accelerometer into active mode using @c active(), and then ask it for a
 *           reading now and then.
 *           \code
 *           IMU* p_sheep = IMU (my_I2C_driver, p_serial);
 *           p_sheep->active ();
 *           ...
 *           *p_serial << PMS ("Y Axis: ") << p_sheep->get_one_axis (1) << endl;
 *           \endcode
 */

class IMU
{
protected:
    /// @brief   Pointer to the I2C port driver used for this accelerometer.
    i2c_master* p_I2C;

#ifdef I2C_DBG
    /// @brief Pointer to a serial device used for showing debugging information.
		emstream* p_serial;
#endif

    /** This is the 8-bit I2C address for the LSM6DSL sensor. This number contains the
     *  7-bit I2C address (either 0x1C or 0x1D) as bits 7:1, and a 0 for the least
     *  significant bit. This is a SLA+W address as described in the AVR datasheets. To
     *  make a read address SLA+R, just or this number with 0x01.
     */
    uint8_t i2c_address;

    /// @brief   Flag which indicates a working LSM6DSL is at the given I2C address.
    bool working;

public:
    // This constructor sets up the driver
    IMU (
            i2c_master* p_I2C_drv, uint8_t address
#ifdef I2C_DBG
            , emstream* p_ser_dev = NULL
#endif
    );

    /** This method sets the I2C address of the device.
     *  @param new_addr The new I2C address to be set
     */
    void set_i2c_address (uint8_t new_addr)
    {
        i2c_address = new_addr;
    }

    // Set up the accelerometer in a default mode
    void initialize (void);

    // Put the LSM6DSL into active mode by setting bit ACTIVE in register CTRL1_XL_REG
    void active (void);

    // Reset the accelerometer using its software reset functionality
    void reset (void);

    // This method reads the current acceleration in one direction: 0 = X, 1 = Y, 2 = Z
    int16_t get_one_axis (uint8_t);

    // Give access to printing operator
    friend emstream& operator << (emstream&, IMU&);
};

// This operator "prints" a _MMA8452Q_H_ sensor by showing its current measured outputs
emstream& operator << (emstream&, IMU&);



// ENABLE TIM4 clock for I2c



/** This constant is used to select the port pin SCL used by IMU.*/
//const uint16_t SCL_PIN = GPIO_Pin_6;  // PB6

/** This constant is used to select the port pin SDA used by IMU.*/
//const uint16_t SDA_PIN = GPIO_Pin_7;  // PB7

/** This definition specifies which GPIO port for SCL and SDA*/
//#define I2C_PORT         GPIOB  // PORT B FROM MEMORY MAP OF PB6 PB7

/** This definition specifies which clock is to be turned on for I2C */
//#define I2C_CLOCK        RCC_AHB1Periph_I2C1  // (PB6/PB7)

/** This macro specifies the pin source used by the I2C. It refers
 *  to the pin number. */
//#define SCL_SOURCE       GPIO_PinSource6 //DETERMINE VALUE (unsure pb6)

/** This macro specifies the pin source used by the I2c. It refers
 *  to the pin number. */
//#define SDA_SOURCE       GPIO_PinSource7 //DETERMINE VALUE (unsure pb7)

/** This macro specifies the timer used.
 */
//#define I2C_TIMER        TIM4  // TIM4 for PB6 and PB7

/** This macro specifies which timer clock is used
 */
//#define I2C_TIMER_CLOCK  RCC_APB1Periph_TIM4 // Tim4 for PB6 and PB7

#endif