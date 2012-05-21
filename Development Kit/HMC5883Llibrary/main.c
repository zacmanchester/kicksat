/*** HMC5883L example program on the ti cc430f5137 ************************************************************

This file initialized the magnetometer and reads the x,z,y magnetometer readings
continuously with a delay between each reading

*******************************************************************************/
#include "cc430f6137.h"
#include "TI_USCI_I2C_master.h"
#include "HMC5883L.h"


volatile int magData[3];




void main(void)
{
  int y = 0;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P1MAP3 = PM_UCB0SDA;                      // Map UCB0SDA output to P2.6
  P1MAP2 = PM_UCB0SCL;                      // Map UCB0SCL output to P2.7
  PMAPPWD = 0;                              // Lock port mapping registers

  P1SEL |= BIT3 + BIT2;                     // Select P2.6 & P2.7 to I2C function

  HMC5883L_initialize();

  while(1)
  {
  HMC5883L_read_once(magData);


  while(y<5000){y++;}
  y=0;

  }

}


