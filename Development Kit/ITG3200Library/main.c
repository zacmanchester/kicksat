/*** ITG3200 example program on the ti cc430f5137 ************************************************************

This file initializes the gyro and reads the x,y,z gyro reading and gyro temperature
continuously with a delay between each reading

Gyro SDA connected to P2.6
gyro SCL connected to P2.7
Gyro VIO connected to HIGH

*******************************************************************************/
#include "cc430f6137.h"
#include "TI_USCI_I2C_master.h"
#include "ITG3200.h"

volatile int gyroData[3];					//3 length integer array to hold gyro output
volatile int gyroTemp;						//integer holding temperature data

void main(void)
{
  int y = 0;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P1MAP3 = PM_UCB0SDA;                      // Map UCB0SDA output to P2.6
  P1MAP2 = PM_UCB0SCL;                      // Map UCB0SCL output to P2.7
  PMAPPWD = 0;                              // Lock port mapping registers

  P1SEL |= BIT3 + BIT2;                     // Select P2.6 & P2.7 to I2C function

  ITG3200_initialize(); 					//initialize gyro

  while(1)
  {
	  ITG3200_read_once(gyroData);  		//read gyro data and store in gyro Data array
	  ITG3200_read_temp(&gyroTemp);

	  while(y<5000){y++;}					//delay for a bit
	  y=0;

  }

}


