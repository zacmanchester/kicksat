/*** HMC5883L magnetometer library ************************************************************

This library provides support for the HMC5883L magnetometer with a cc430f5137 TI microcontroller

*******************************************************************************/
#include "TI_USCI_I2C_master.h"
#include "ITG3200.h"
#include "ITG3200_Config.h"

unsigned char sendGyro[2] = {0, 0};  //sendGyro vector, first value register address, second value data to be sent
volatile unsigned char RxBufferGyro[6];
unsigned char itg3200Address = GYRO_ADDRESS;
unsigned char prescaleGyro = 12;
//unsigned char recieve[6] = {0,0,0,0,0,0}; not needed i think
//int countGyro = 0; not needed

void ITG3200_initialize(){
/*this function initializes the itg3200 gyro for continuous reading mode with a noise filter,
 * specified sampling rate, specified XXXXXX, specified XXXX, and specified XXXXX
 * from the itg3200 config header file.
 */

	sendGyro[0] = SMPL_RATE_REG_ADDR; sendGyro[1] = (GYRO_SAMPLE_RATE);				//Sample rate divider registers
																					//samples rate divider relative to internal sampling
	TI_USCI_I2C_transmitinit(itg3200Address,prescaleGyro);  //initiate transmission
	TI_USCI_I2C_transmit(2,sendGyro);						//write to register

	sendGyro[0] = DLPF_RANGE_REG_ADDR;												//Sample rate divider registers
	sendGyro[1] = GYRO_RANGE|GYRO_FILTER_SMPL_RATE ;								//samples rate divider relative to internal sampling

	TI_USCI_I2C_transmit(2,sendGyro);						//write to DLPF and range register
}

void ITG3200_read_once(unsigned int *store){
	/* This function reads 6 bytes from the gyro cooresponding to the X,Y, and Z
	 * angular velocity readings in 2's compliment. Pointer to a length 3 integer array is passed to store the values
	 */
	sendGyro[0] = 0x1D;													//point to first gyro data register
	TI_USCI_I2C_transmitinit(itg3200Address, prescaleGyro);
	TI_USCI_I2C_transmit(1, sendGyro);

	TI_USCI_I2C_receiveinit(itg3200Address,prescaleGyro);   				// receiving 6 bytes
	TI_USCI_I2C_receive(6, RxBufferGyro);

	*store = ((int)RxBufferGyro[0]<<8) + (int)RxBufferGyro[1];      	//X reading
	*(store+1) = ((int)RxBufferGyro[2]<<8) + (int)RxBufferGyro[3];  	//Y reading
	*(store+2) = ((int)RxBufferGyro[4]<<8) + (int)RxBufferGyro[5];  	//Z reading
}

void ITG3200_read_temp(unsigned int *tempStore){
/*This function reads the temperature from the gyro temperature sensor and stores in an integer pointer
 *
 */

	sendGyro[0] = 0x1B;													//point to first temperature data register
	TI_USCI_I2C_transmitinit(itg3200Address, prescaleGyro);
	TI_USCI_I2C_transmit(1, sendGyro);

	TI_USCI_I2C_receiveinit(itg3200Address,prescaleGyro);   				// receiving 6 bytes
	TI_USCI_I2C_receive(2, RxBufferGyro);

	*tempStore = (((int)RxBufferGyro[0]<<8) + (int)RxBufferGyro[1])+13000;  //2's compliment temperature reading subtracted from offset

}
