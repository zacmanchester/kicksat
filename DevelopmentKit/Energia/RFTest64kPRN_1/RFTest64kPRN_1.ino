#include <CC430Radio.h>

/*
  This example code will configure the CC1101 radio core in the CC430 to continuously transmit a PRN sequence.
  The output signal will be MSK modulated at 64 kbps on a 437.5 MHz carrier.
*/

static CC1101Settings rfSettings = {
    0x0E,   // FSCTRL1
    0x00,   // FSCTRL0
    0x10,   // FREQ2
    0xD3,   // FREQ1
    0xB1,   // FREQ0
    0x0B,   // MDMCFG4
    0x43,   // MDMCFG3
    0x70,   // MDMCFG2
    0x02,   // MDMCFG1
    0xF8,   // MDMCFG0
    0x00,   // CHANNR
    0x07,   // DEVIATN
    0xB6,   // FREND1
    0x10,   // FREND0
    0x18,   // MCSM0
    0x1D,   // FOCCFG
    0x1C,   // BSCFG
    0xC7,   // AGCCTRL2
    0x00,   // AGCCTRL1
    0xB0,   // AGCCTRL0
    0xEA,   // FSCAL3
    0x2A,   // FSCAL2
    0x00,   // FSCAL1
    0x1F,   // FSCAL0
    0x59,   // FSTEST
    0x88,   // TEST2
    0x31,   // TEST1
    0x09,   // TEST0
    0x07,   // FIFOTHR
    0x29,   // IOCFG2
    0x06,   // IOCFG0
    0x00,   // PKTCTRL1  Packet Automation (0x04 = append status bytes)
    0x02,   // PKTCTRL0  0x02 = infinite packet length, 0x00 = Fixed Packet Size, 0x40 = whitening, 0x20 = PN9
    0x00,   // ADDR      Device address.
    0xFF    // PKTLEN    Packet Length (Bytes)
};

static unsigned char prn[64] = {
  0b10101010, 0b11110111, 0b11100111, 0b10100100, 0b11111001, 0b01111101, 0b00000010, 0b11000100,
  0b11001110, 0b11110101, 0b10110111, 0b01010110, 0b10010111, 0b01000100, 0b00010000, 0b11010111,
  0b01100001, 0b00011111, 0b11110001, 0b10000010, 0b10001110, 0b11010000, 0b10111000, 0b01110000,
  0b01110100, 0b11010101, 0b00110001, 0b11101101, 0b10011111, 0b10111011, 0b10011100, 0b11000011,
  0b00010111, 0b10011010, 0b00110010, 0b00000001, 0b00101000, 0b01111001, 0b00110111, 0b00011100,
  0b10001001, 0b00001001, 0b11010111, 0b11110101, 0b00100011, 0b01101010, 0b00001100, 0b11001010,
  0b01010100, 0b01010011, 0b10001010, 0b11100101, 0b01100000, 0b00110111, 0b10001000, 0b10110101,
  0b10010010, 0b01011001, 0b10110001, 0b10100111, 0b10000001, 0b11110110, 0b01011011, 0b11100000
};

void setup() {
  pinMode(5, OUTPUT);
};

void loop() {
  Radio.reset();
  
  Radio.writeConfiguration(&rfSettings);  // Write settings to configuration registers
  
  Radio.writePATable(0xC3);  // 10 dBm output power
  
  Radio.writeTXBuffer(prn, 64); // write prn to transmit buffer
  
  Radio.strobe(RF_STX);  // Turn on transmitter
  
  //Blink LED, wait a second
  digitalWrite(5, HIGH);
  delay(100);
  digitalWrite(5, LOW);
  delay(400);
};



