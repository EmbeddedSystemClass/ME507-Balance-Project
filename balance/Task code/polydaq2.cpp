//*************************************************************************************
/** @file polydaq2.cpp
 *    This file contains the headers for a class that interacts with the hardware on a
 *    PolyDAQ 2 board. 
 *
 *  Revisions:
 *    @li 09-03-2014 JRR Original file
 *    @li 03-18-2015 JRR Added code to work with the MMA8452Q accelerometer
 *
 *  License:
 *    This file is copyright 2015 by JR Ridgely and released under the Lesser GNU 
 *    Public License, version 3. It intended for educational use only, but its use
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

#include "balance.h"


//-------------------------------------------------------------------------------------
/** @brief   Create a PolyDAQ 2 interface object.
 *  @details This constructor creates a new PolyDAQ 2 driver object. It saves pointers
 *           and creates new analog to digital and digital to analog converter driver
 *           objects.
 *  @param   serpt A pointer to a serial port that can be used for debugging (default:
 *                 NULL, meaning no debugging messages will be sent).
 */

polydaq2::polydaq2 (emstream* serpt = NULL)
{
	// Save the serial port pointer
	p_serial = serpt;



	// Set up the I2C driver
	p_i2c = new i2c_master (GPIOB, 6, 7, serpt);

	// Set up the MMA8452Q accelerometer on the I2C bus, if it's there
	#ifdef PDQ_MMA8452Q_ADDR
		p_accel = new mma8452q (p_i2c, PDQ_MMA8452Q_ADDR, p_serial);
	#else
		p_accel = NULL;
	#endif
	#ifdef PDQ_EXT_MMA8452Q_ADDR
		p_ext_accel = new mma8452q (p_i2c, PDQ_EXT_MMA8452Q_ADDR, p_serial);
	#else
		p_ext_accel = NULL;
	#endif
}


//-------------------------------------------------------------------------------------
/** @brief   Initialize components which need to be set up after the RTOS is started.
 *  @details This method sets up PolyDAQ 2 components which cannot be set up until the
 *           RTOS has been started. For example, and A/D converter and anything on the
 *           I2C bus can only be accessed with the RTOS running because a mutex is 
 *           used to protect each such component and RTOS delays may be used in some
 *           code. 
 */

void polydaq2::initialize (void)
{
	/////////////////////////////////////////////////////
	p_i2c->scan (p_serial);
	/////////////////////////////////////////////////////

	// Initialize each accelerometer; if either is absent, its working flag goes false
	if (p_accel)
	{
		p_accel->initialize ();
	}
	if (p_ext_accel)
	{
		p_ext_accel->initialize ();
	}
}



//-------------------------------------------------------------------------------------
/** @brief   Read one channel of the PolyDAQ's accelerometer.
 *  @details This function reads one channel of the accelerometer, if one is present.
 *           If the PolyDAQ is configured not to use an accelerometer, this method 
 *           returns zero.
 *  @param   channel The accelerometer channel to read: X = 0, Y = 1, and Z = 2
 *  @return  The accelerometer reading from the given channel or 0 if there's no 
 *           accelerometer
 */

int16_t polydaq2::get_accel (uint8_t channel)
{
	if (p_accel && (channel <= 2))
	{
		return (p_accel->get_one_axis (channel));
	}
	return (0);
}


//-------------------------------------------------------------------------------------
/** @brief   Read one channel of an externally attached MMA8452Q accelerometer.
 *  @details This function reads one channel of the external accelerometer, if one is 
 *           present. If the PolyDAQ is configured not to use an accelerometer or there
 *           is no external accelerometer connected, this method returns zero.
 *  @param   channel The accelerometer channel to read: x = 0, y = 1, and z = 2
 *  @return  The accelerometer reading from the given channel or 0 if there's no 
 *           accelerometer there
 */

int16_t polydaq2::get_ext_accel (uint8_t channel)
{
	if (p_accel && (channel <= 2))
	{
		return (p_ext_accel->get_one_axis (channel));
	}
	return (0);
}


