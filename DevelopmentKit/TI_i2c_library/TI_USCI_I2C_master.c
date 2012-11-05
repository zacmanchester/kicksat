//******************************************************************************
//   MSP430 USCI I2C Transmitter and Receiver
//
//  Description: This code configures the CC430's USCI module as 
//  I2C master capable of transmitting and receiving bytes. 
//
//  ***THIS IS THE MASTER CODE***
//
//                    Master                   
//                 CC430Fx137             
//             -----------------          
//         /|\|              XIN|-   
//          | |                 |     
//          --|RST          XOUT|-    
//            |                 |        
//            |                 |        
//            |                 |       
//            |         SDA/P3.1|------->
//            |         SCL/P3.2|------->
//
// Note: External pull-ups are needed for SDA & SCL
//******************************************************************************
#include "cc430f6137.h"
#include "signal.h"  //needed for interrupt syntax with mspgcc4

#include "TI_USCI_I2C_master.h"

unsigned char *PTxData;                     // Pointer to TX data
unsigned char TXByteCtr;

unsigned char *PRxData;                     // Pointer to RX data
unsigned char RXByteCtr;

//------------------------------------------------------------------------------
// void TI_USCI_I2C_receiveinit(unsigned char slave_address, 
//                              unsigned char prescale)
//
// This function initializes the USCI module for master-receive operation. 
//
// IN:   unsigned char slave_address   =>  Slave Address
//       unsigned char prescale        =>  SCL clock adjustment 
//-----------------------------------------------------------------------------
void TI_USCI_I2C_receiveinit(unsigned char slave_address, 
                             unsigned char prescale){
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset THESE TWO NEEDED TO RESET FOR READ
	UCB0BR0 = prescale;                             // fSCL = SMCLK/12 = ~100kHz
	UCB0BR1 = 0;
    UCB0I2CSA = slave_address;                         // Slave Address 
    UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
	UCB0IE |= UCRXIE;                         // Enable RX interrupt
}

//------------------------------------------------------------------------------
// void TI_USCI_I2C_transmitinit(unsigned char slave_address, 
//                               unsigned char prescale)
//
// This function initializes the USCI module for master-transmit operation. 
//
// IN:   unsigned char slave_address   =>  Slave Address
//       unsigned char prescale        =>  SCL clock adjustment 
//------------------------------------------------------------------------------
void TI_USCI_I2C_transmitinit(unsigned char slave_address, 
                          unsigned char prescale){
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = prescale;                             // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = slave_address;                         // Slave Address
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  UCB0IE |= UCTXIE;                         // Enable TX interrupt
}

//------------------------------------------------------------------------------
// void TI_USCI_I2C_receive(unsigned char byteCount, unsigned char *field)
//
// This function is used to start an I2C commuincation in master-receiver mode. 
//
// IN:   unsigned char byteCount  =>  number of bytes that should be read
//       unsigned char *field     =>  array variable used to store received data
//------------------------------------------------------------------------------
void TI_USCI_I2C_receive(unsigned char byte_count, unsigned char *field){
	PRxData = field;					    // Start of RX buffer
    RXByteCtr = byte_count;                          // Load RX byte counter
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition

    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0, enable interrupts
        	                                     // Remain in LPM0 until all data
        	                                     // is RX'd
    __no_operation();                       // Set breakpoint >>here<< and
        	                                     // read out the RxBuffer buffer
}

//------------------------------------------------------------------------------
// void TI_USCI_I2C_transmit(unsigned char byteCount, unsigned char *field)
//
// This function is used to start an I2C commuincation in master-transmit mode. 
//
// IN:   unsigned char byteCount  =>  number of bytes that should be transmitted
//       unsigned char *field     =>  array variable. Its content will be sent.
//------------------------------------------------------------------------------
void TI_USCI_I2C_transmit(unsigned char byte_count, unsigned char *field){
    PTxData = field;   					    // TX array start address
                                            // Place breakpoint here to see each
                                            // transmit operation.
    TXByteCtr = byte_count;                 // Load TX byte counter

    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0, enable interrupts
    __no_operation();                       // Remain in LPM0 until all data
                                            // is TX'd
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
}



//------------------------------------------------------------------------------
// The USCIAB0TX_ISR is structured such that it can be used to transmit any
// number of bytes by pre-loading TXByteCtr with the byte count. Also, TXData
// points to the next byte to transmit.

// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
// any 2+ number of bytes by pre-loading RXByteCtr with the byte count.
//------------------------------------------------------------------------------
interrupt(USCI_B0_VECTOR) USCI_B0_ISR(void)
{
  //switch(__even_in_range(UCB0IV,12))
   switch(UCB0IV)
  {
  case  0: break;                            // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4: break;                           // Vector  4: NACKIFG
  case  6: break;                           // Vector  6: STTIFG
  case  8: break;                           // Vector  8: STPIFG
  case 10:                                 // Vector 10: RXIFG
	 RXByteCtr--;                            // Decrement RX byte counter
	 if (RXByteCtr)
	 {
	    *PRxData++ = UCB0RXBUF;               // Move RX data to address PRxData
	    if (RXByteCtr == 1)                   // Only one byte left?
	        UCB0CTL1 |= UCTXSTP;                // Generate I2C stop condition
	    }
	    else
	    {
	        *PRxData = UCB0RXBUF;                 // Move final RX data to PRxData
	        __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
	    }
	    break;

  case 12:                                  // Vector 12: TXIFG
    if (TXByteCtr)                          // Check TX byte counter
    {
      UCB0TXBUF = *PTxData++;               // Load TX buffer
      TXByteCtr--;                          // Decrement TX byte counter
    }
    else
    {
      UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
      UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
      __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
    }
  default: break;
  }
}

