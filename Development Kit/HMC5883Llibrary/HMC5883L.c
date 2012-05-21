/*** HMC5883L magnetometer library ************************************************************

This library provides support for the HMC5883L magnetometer with a cc430f5137 TI microcontroller

*******************************************************************************/
#include "TI_USCI_I2C_master.h"
#include "HMC5883L.h"
#include "HMC5883L_Config.h"

unsigned char sendMag[2] = {0, 0};  //sendMag vector, first value register address, second value data to be sent
volatile unsigned char RxBufferMag[6];
unsigned char magAddress = 0x1E;
unsigned char prescaleMag = 12;
int count = 0;


void HMC5883L_initialize(){
/*this function initializes the magnetometer for continuous reading mode with a specified gain,
 * specified sampling rate, specified XXXXXX, specified XXXX, and specified XXXXX
 * from the magnetometer config header file.
 */


	sendMag[0] = 0x00; sendMag[1] = (MAG_SAMPLES_AVE|MAG_DATA_RATE|MAG_MEAS_MODE);	//Configuration Register A (CRA)
																					//samples averaged per reading, data output rate
	TI_USCI_I2C_transmitinit(magAddress,prescaleMag);   //initiate transmission
	TI_USCI_I2C_transmit(2,sendMag);					//write to CRA

	sendMag[0] = 0x01; sendMag[1] = MAG_GAIN;			//CRB, gain setting
	TI_USCI_I2C_transmit(2,sendMag);					//write to CRB

	sendMag[0] = 0x02; sendMag[1] = MAG_OPER_MODE;		//Mode register, operating mode
	TI_USCI_I2C_transmit(2,sendMag);					//write to CRB
}

void HMC5883L_read_once(unsigned int *store){
	/* This function reads 6 bytes from the magnetometer cooresponding to the X,Z, and Y
	 * magnetic strength readings. Pointer to a length 3 integer array is passed to store the values
	 */

	sendMag[0] = 0x03;

	TI_USCI_I2C_transmitinit(magAddress,prescaleMag);   //Point to first data register for reading
	TI_USCI_I2C_transmit(1,sendMag);

	TI_USCI_I2C_receiveinit(magAddress,prescaleMag);   				// receiving 6 bytes
	TI_USCI_I2C_receive(6, RxBufferMag);

	*store = ((int)RxBufferMag[0]<<8) + (int)RxBufferMag[1];      	//X reading
	*(store+1) = ((int)RxBufferMag[2]<<8) + (int)RxBufferMag[3];  	//Z reading
	*(store+2) = ((int)RxBufferMag[4]<<8) + (int)RxBufferMag[5];  	//Y reading


}
