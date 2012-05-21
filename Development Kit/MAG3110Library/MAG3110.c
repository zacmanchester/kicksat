/*** HMC5883L magnetometer library ************************************************************

This library provides support for the mag3110 magnetometer with a cc430f5137 TI microcontroller

*******************************************************************************/
#include "TI_USCI_I2C_master.h"
#include "MAG3110.h"
#include "MAG3110_Config.h"

unsigned char sendMag3110[2] = {0, 0};  //sendMag vector, first value register address, second value data to be sent
volatile unsigned char RxBufferMag3110[6];
unsigned char mag3110Address = 0x0E;	//7 bit i2c address
unsigned char prescaleMag3110 = 12;
int countMag3110 = 0;

void MAG3110_initialize(){
/* This function initializes the mag3110 magnetometer for continuous reading mode
 */


	sendMag3110[0] = CTRL_REG2_ADDR;							//Control Register 2
	sendMag3110[1] = CTRL_REG_2_WRITE;							//Control Register 2 bits

	TI_USCI_I2C_transmitinit(mag3110Address,prescaleMag3110);   //initiate transmission
	TI_USCI_I2C_transmit(2,sendMag3110);					    //write to CTRL REG 2

	sendMag3110[0] = CTRL_REG1_ADDR;							//Control Register 1
	sendMag3110[1] = CTRL_REG_1_WRITE;							//Control Register 1 bits

	TI_USCI_I2C_transmit(2,sendMag3110);						//write to CTRL REG 1



}

void MAG3110_read_once(unsigned int *store){
	/* This function reads 6 bytes from the magnetometer cooresponding to the X,Y, and Z
	 * magnetic strength readings. Pointer to a length 3 integer array is passed to store the values
	 */
	sendMag3110[0] = 0x01;								//first data register address with auto increment

	TI_USCI_I2C_transmitinit(mag3110Address,prescaleMag3110);   //Point to first data register for future reading
	TI_USCI_I2C_transmit(1,sendMag3110);

	TI_USCI_I2C_receiveinit(mag3110Address,prescaleMag3110);   				// receiving 6 bytes
	TI_USCI_I2C_receive(6, RxBufferMag3110);

	*store = ((int)RxBufferMag3110[0]<<8) + (int)RxBufferMag3110[1];      	//X reading
	*(store+1) = ((int)RxBufferMag3110[2]<<8) + (int)RxBufferMag3110[3];  	//Y reading
	*(store+2) = ((int)RxBufferMag3110[4]<<8) + (int)RxBufferMag3110[5];  	//Z reading


}
