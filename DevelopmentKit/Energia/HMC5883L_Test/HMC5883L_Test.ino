 /*
  Magnetometer Test
 */

void setup() {
  
  /* Port Mapping for I2C */
  UCB0CTL1 |= UCSWRST;
  PMAPPWD = 0x02D52;	// Get write-access to port mapping regs
  PMAPCTL = PMAPRECFG;
  P2MAP0 = PM_UCB0SCL;	// Map SCL input to P2.0 
  P2MAP1 = PM_UCB0SDA;	// Map SDA output to P2.1 
  PMAPPWD = 0;		// Lock port mapping registers
  P2SEL |= BIT0 | BIT1;	//Set P1.1 and P1.2 to USCI Mode
  
  delay(10);
}

void loop() {
  
  delay(1000);
}

void txconfig() {
  UCB0CTL1 |= UCSWRST;                   // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;  // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;         // Use SMCLK, keep SW reset
  UCB0BR0 = 80;                          // fSCL = SMCLK/80 = 100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = 0x1E;                      // Slave Address
  UCB0CTL1 &= ~UCSWRST;                  // Clear SW reset, resume operation
  UCB0IE |= UCTXIE;                      // Enable TX interrupt
}

void rxconfig() {
  UCB0CTL1 |= UCSWRST;                   // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;  // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;         // Use SMCLK, keep SW reset THESE TWO NEEDED TO RESET FOR READ
  UCB0BR0 = 80;                          // fSCL = SMCLK/80 = 100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = 0x1E;                      // Slave Address 
  UCB0CTL1 &= ~UCSWRST;                  // Clear SW reset, resume operation
  UCB0IE |= UCRXIE;                      // Enable RX interrupt
}
