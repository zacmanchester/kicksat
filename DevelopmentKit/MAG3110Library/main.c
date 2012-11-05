/*** HMC5883L example program on the ti cc430f5137 ************************************************************

This file initializes the MAG3110 magnetometer and reads the x,z,y magnetometer readings
continuously with a delay between each reading

mag SDA connected to P2.6
mag SCL connected to P2.7
*******************************************************************************/
#include "cc430f6137.h"
#include "TI_USCI_I2C_master.h"
#include "MAG3110.h"


volatile int mag3110Data[3];





void main(void)
{
  int y = 0;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P1MAP3 = PM_UCB0SDA;                      // Map UCB0SDA output to P2.6
  P1MAP2 = PM_UCB0SCL;                      // Map UCB0SCL output to P2.7
  PMAPPWD = 0;                              // Lock port mapping registers

  P1SEL |= BIT3 + BIT2;                     // Select P2.6 & P2.7 to I2C function

  MAG3110_initialize();

  while(1)
  {
  MAG3110_read_once(mag3110Data);


  while(y<5000){y++;}
  y=0;

  }

}


