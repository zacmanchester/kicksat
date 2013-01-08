#define magAddress 0x1E //0011110b, I2C 7bit address of HMC5883
#define MAG_GAIN 0b00000000 //0.73mG/LSb Range = +-0.88Ga
#define MAG_SAMPLES_AVE 0b00000000 //1 sample --default
#define MAG_DATA_RATE 0b00010000 //15Hz -- default
#define MAG_MEAS_MODE 0b00000000
#define MAG_OPER_MODE 0b00000000 //continuous measurement mode --default

unsigned char sendMag[2] = {0, 0};
unsigned char RxBufferMag[6];

void setup(){
  
  /* Port Mapping for I2C */
  UCB0CTL1 |= UCSWRST;
  PMAPPWD = 0x02D52;	// Get write-access to port mapping regs
  PMAPCTL = PMAPRECFG;  // Allow future runtime port mapping
  P2MAP0 = PM_UCB0SCL;	// Map SCL input to P2.0 
  P2MAP1 = PM_UCB0SDA;	// Map SDA output to P2.1 
  PMAPPWD = 0;		// Lock port mapping registers
  P2SEL |= BIT0 | BIT1;	//Set P2.0 and P2.1 to USCI Mode
  
  //Initialize Serial and I2C communications
  Serial.begin(9600);
  delay(1000);
  Serial.println("Hello");
  
  sendMag[0] = 0x00;
  sendMag[1] = (MAG_SAMPLES_AVE|MAG_DATA_RATE|MAG_MEAS_MODE);	//Configuration Register A (CRA)
																					//samples averaged per reading, data output rate
  TI_USCI_I2C_transmitinit(magAddress,12);   //initiate transmission
  TI_USCI_I2C_transmit(2,sendMag);					//write to CRA

  sendMag[0] = 0x01; sendMag[1] = MAG_GAIN;			//CRB, gain setting
  TI_USCI_I2C_transmit(2,sendMag);					//write to CRB

  sendMag[0] = 0x02; sendMag[1] = MAG_OPER_MODE;		//Mode register, operating mode
  TI_USCI_I2C_transmit(2,sendMag);					//write to CRB

}

int x, y, z;

void loop(){
  
  sendMag[0] = 0x03;

  TI_USCI_I2C_transmitinit(magAddress,12);   //Point to first data register for reading
  TI_USCI_I2C_transmit(1,sendMag);

  TI_USCI_I2C_receiveinit(magAddress,12);   // receiving 6 bytes
  TI_USCI_I2C_receive(6, RxBufferMag);

  x = ((int)RxBufferMag[0]<<8) + (int)RxBufferMag[1];   //X reading
  y = ((int)RxBufferMag[2]<<8) + (int)RxBufferMag[3];  	//Z reading
  z = ((int)RxBufferMag[4]<<8) + (int)RxBufferMag[5];  	//Y reading
  
  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("  y: ");
  Serial.print(y);
  Serial.print("  z: ");
  Serial.println(z);
  
  delay(500);
}
