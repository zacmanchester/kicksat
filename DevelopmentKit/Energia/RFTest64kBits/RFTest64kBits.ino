#include <SpriteRadio.h>

/*
  This example code will configure the CC1101 radio core in the CC430 to continuously transmit a PRN sequence.
  The output signal will be MSK modulated at 64 kbps on a 437.5 MHz carrier.
*/

CC1101Settings rfSettings = {
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

unsigned char prn1[64] = {
  0b10101010, 0b11110111, 0b11100111, 0b10100100, 0b11111001, 0b01111101, 0b00000010, 0b11000100,
  0b11001110, 0b11110101, 0b10110111, 0b01010110, 0b10010111, 0b01000100, 0b00010000, 0b11010111,
  0b01100001, 0b00011111, 0b11110001, 0b10000010, 0b10001110, 0b11010000, 0b10111000, 0b01110000,
  0b01110100, 0b11010101, 0b00110001, 0b11101101, 0b10011111, 0b10111011, 0b10011100, 0b11000011,
  0b00010111, 0b10011010, 0b00110010, 0b00000001, 0b00101000, 0b01111001, 0b00110111, 0b00011100,
  0b10001001, 0b00001001, 0b11010111, 0b11110101, 0b00100011, 0b01101010, 0b00001100, 0b11001010,
  0b01010100, 0b01010011, 0b10001010, 0b11100101, 0b01100000, 0b00110111, 0b10001000, 0b10110101,
  0b10010010, 0b01011001, 0b10110001, 0b10100111, 0b10000001, 0b11110110, 0b01011011, 0b11100000
};
unsigned char prn2[64] = {
  0b10101010, 0b10000001, 0b01001010, 0b11110010, 0b11101110, 0b00000111, 0b00111010, 0b01001111,
  0b01011101, 0b01000100, 0b10000110, 0b01110000, 0b10111101, 0b10110011, 0b01000011, 0b10111100,
  0b00111111, 0b11100000, 0b11110111, 0b11000101, 0b11001100, 0b10000010, 0b01010011, 0b10110100,
  0b01111001, 0b11110011, 0b01100010, 0b10100100, 0b01110001, 0b10110101, 0b01110001, 0b00110001,
  0b00010000, 0b00001000, 0b01000110, 0b00010011, 0b10010101, 0b01100001, 0b10111101, 0b00110111,
  0b00100010, 0b10000101, 0b01101001, 0b11111011, 0b00100100, 0b10110111, 0b11100100, 0b11010100,
  0b11001100, 0b00000110, 0b00110010, 0b10001101, 0b00101111, 0b11101000, 0b10110001, 0b11010110,
  0b01011001, 0b11100011, 0b11101110, 0b10000011, 0b01011011, 0b01110110, 0b00001011, 0b01011110
};

SpriteRadio m_radio;

void setup() {
  pinMode(5, OUTPUT);
  m_radio = SpriteRadio();
  m_radio.txInit();
};


void loop() {
  
  //Blink LED, wait a second
  digitalWrite(5, HIGH);
  m_radio.transmit(prn1,64);
  delay(300);
  digitalWrite(5, LOW);
  delay(700);
};


void txInit() {
  
  char status;
  
  Radio.reset();
  Radio.writeConfiguration(&rfSettings);  // Write settings to configuration registers
  Radio.writePATable(0xC3);  // 10 dBm output power
  
  //Put radio into idle state
  status = Radio.strobe(RF_SIDLE);
  while (status & 0xF0)
  {
    status = Radio.strobe(RF_SNOP);
  }
}

void transmit(unsigned char bytes[], unsigned int length) {
  
  char status;

  //Wait for radio to be in idle state
  status = Radio.strobe(RF_SIDLE);
  while (status & 0xF0)
  {
    status = Radio.strobe(RF_SNOP);
  }
  
  //Clear TX FIFO
  status = Radio.strobe(RF_SFTX);
  
  if(length <= 64)
  {
    Radio.writeTXBuffer(bytes, length); //Write bytes to transmit buffer
    status = Radio.strobe(RF_STX);  //Turn on transmitter
  }
  else
  {
    unsigned char bytes_free, bytes_to_write;
    unsigned int bytes_to_go, counter;
    
    Radio.writeTXBuffer(bytes, 64); //Write first 64 bytes to transmit buffer
    bytes_to_go = length - 64;
    counter = 64;
    
    status = Radio.strobe(RF_STX);  //Turn on transmitter
    
    //Wait for oscillator to stabilize
    while (status & 0xC0)
    {
      status = Radio.strobe(RF_SNOP);
    }
    
    while(bytes_to_go)
    {
      Serial.println(bytes_to_go);
      delay(1); //Wait for some bytes to be transmitted
    
      bytes_free = Radio.strobe(RF_SNOP) & 0x0F;
      bytes_to_write = bytes_free < bytes_to_go ? bytes_free : bytes_to_go;
    
      Radio.writeTXBuffer(bytes+counter, bytes_to_write);
      bytes_to_go -= bytes_to_write;
      counter += bytes_to_write;
    }
  }
  
  //Wait for transmission to finish
  while(status != 0x7F)
  {
    status = Radio.strobe(RF_SNOP);
  }
  Radio.strobe(RF_SIDLE); //Put radio back in idle mode
  return;
}
