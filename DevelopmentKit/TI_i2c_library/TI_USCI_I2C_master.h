#ifndef USCI_LIB
#define USCI_LIB

void TI_USCI_I2C_receiveinit(unsigned char slave_address, unsigned char prescale);
void TI_USCI_I2C_transmitinit(unsigned char slave_address, unsigned char prescale);


void TI_USCI_I2C_receive(unsigned char byte_count, unsigned char *field);
void TI_USCI_I2C_transmit(unsigned char byte_count, unsigned char *field);


#endif
