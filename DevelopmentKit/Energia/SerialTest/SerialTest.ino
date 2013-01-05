 /*
  SerialTest
 */

void setup() {
  
  /* USCI/UART Setup */
  UCA0CTL1 |= UCSWRST;  // Put the USCI module in reset
  UCA0CTL1 |= UCSSEL_2; // Select SMCLK as the UART clock
  
  UCA0BR0 = 52;    // Integer part of UART frequency scaler (low byte)
  UCA0BR1 = 0;     // Integer part of UART frequency scaler (high byte)
  UCA0MCTL = UCBRF_1 + UCBRS_0 + UCOS16; // This turns on oversampling and sets the decimal part of the scaler
  
  /* Port Mapping */
  PMAPPWD = 0x02D52;	// Get write-access to port mapping regs  
  P1MAP1 = PM_UCA0RXD;	// Map UCA0RXD input to P1.1 
  P1MAP2 = PM_UCA0TXD;	// Map UCA0TXD output to P1.2 
  PMAPPWD = 0;		// Lock port mapping registers

  P1DIR |= BIT2;	//Set P1.2 to output
  P1SEL |= BIT1 | BIT2;	//Set P1.1 and P1.2 to USCI Mode
  
  UCA0CTL1 &= ~UCSWRST;  // Take the USCI module out of reset
  delay(10);
}

void loop() {
  UCA0TXBUF = 'H';
  delay(1);
  UCA0TXBUF = 'i';
  delay(1);
  UCA0TXBUF = '!';
  delay(1);
  UCA0TXBUF = '\n';
  delay(1000);
}
