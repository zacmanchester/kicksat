#ifndef MAG3110_CONFIG_H_
#define MAG3110_CONFIG_H_


/* This header contains all of the various configurations for the mag3110 magnetometer
 * Select a configuration by uncommenting the appropriate #define(s).
 */

/*----------------------Control Register 1------------------------------------*/
/*Choose the magnetometer settings. Only uncomment one in each block.
 */

// -------------------DATA RATE SELECTION AND SAMPLING RATIO--------------------------
#define MAG3110_DATA_RATE 0b01100000 	// currently set to an output rate of 10Hz over a sampling ratio of 1
//
/* ****************************NOTE*******************************
* Please reference Table 30 on page 15 of the data sheet for the Mag3110 magnetometer for a complete
* table listing of output rates and sampling ratios for this magnetometer.
* Only change bits 7 through 3 which correspond to DR2 DR1 DR0 OS0 OS1
* In general, power consumption increases with increasing output rate, and with increasing sampling ratio
*
* **************************SENSITIVITY NOTE**************************
* The sensitivity of this magnetometer is 0.01 microTesla/LSB
* In order to use this magetometer to measure magnetic fields, accurate zero offsets must be provided
*
*/


// -------------------FAST READ-------------------------
//Choose whether to read the full 16 bit values, or do a fast read, reading 8 bit values from the MSB registers
//NOTE -- Fast read functionality not currently implemented and likely not needed for this magnetometers purpose
#define MAG3110_FAST_READ 0b00000000	 //fast read off
//#define MAG3110_FAST_READ 0b00000100	 //fast read on
//------------------------------------------------------

// -------------------TRIGGER MEASUREMENT-------------------------
//Choose whether to use a triggered measurement, where it starts in standby mode, reads measurement,
//and returns to standby mode. Default off.
#define  MAG3110_TRIG_MEAS 0b00000000	//Normal operation
//#define  MAG3110_TRIG_MEAS 0b00000010	//trigger measurement
/* ****************************NOTE*******************************
* Functionality not currently implemented as it is not currently needed
*
*/

// -------------------STANBY/ACTIVE-------------------------
//Choose to have the magnetometer in standby or active mode
#define MAG3110_STANDBY 0b00000001		 //Active mode
//#define MAG3110_STANDBY 0b00000000	 //Standby mode
//------------------------------------------------------

#define CTRL_REG_1_WRITE (MAG3110_STANDBY|MAG3110_TRIG_MEAS|MAG3110_FAST_READ|MAG3110_DATA_RATE)


/*----------------------Control Register 2------------------------------------*/
/*Choose the magnetometer settings. Only uncomment one in each block.
 *
 */

// Automatic Magnetic Sensor Reset--------------------------
#define MAG3110_SENS_RST 0b10000000 	// magnetic sensor reset cycle before each data acquisition enabled
										// suggested to be explicitly enabled
//#define MAG3110_SENS_RST 0b00000000	// sensor reset disabled
//----------------------------------------------------------



// DATA OUTPUT CORRECTION----------------------------------
//#define MAG3110_RAW 0b00000000 			// Normal mode, data values corrected by user offset register values
#define MAG3110_RAW 0b00100000 	// raw mode, sensor data values not offset by user offset register values
//----------------------------------------------------------



// Magnetic Sensor Reset-----------------------------------
//initiates a magnetic sensor reset cycle that will restore correct operation after exposure to excessive magnetic
//field which exceeds full scale range
#define MAG3110_RST 0b00000000 	    // Reset cycle not active
//#define MAG3110_RST 0b00010000 	// Reset cycle initiate
//*************************NOTE****************************
//LEAVE OFF, FUNCTIONALITY NOT CURRENTLY IMPLEMETED AND ALREADY DONE AUTOMATICALLY BY MAG3110_SENS_RST CONFIG
//----------------------------------------------------------

#define CTRL_REG_2_WRITE (MAG3110_SENS_RST|MAG3110_RAW|MAG3110_RST)



#endif
