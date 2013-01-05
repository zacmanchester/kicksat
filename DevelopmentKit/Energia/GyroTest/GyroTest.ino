//This is a list of registers in the ITG-3200. Registers are parameters that determine how the sensor will behave, or they can hold data that represent the
//sensors current status.
//To learn more about the registers on the ITG-3200, download and read the datasheet.
char WHO_AM_I = 0x00;
char SMPLRT_DIV= 0x15;
char DLPF_FS = 0x16;
char GYRO_XOUT_H = 0x1D;
char GYRO_XOUT_L = 0x1E;
char GYRO_YOUT_H = 0x1F;
char GYRO_YOUT_L = 0x20;
char GYRO_ZOUT_H = 0x21;
char GYRO_ZOUT_L = 0x22;

//This is a list of settings that can be loaded into the registers.
//DLPF, Full Scale Register Bits
//FS_SEL must be set to 3 for proper operation
//Set DLPF_CFG to 3 for 1kHz Fint and 42 Hz Low Pass Filter
char DLPF_CFG_0 = 1<<0;
char DLPF_CFG_1 = 1<<1;
char DLPF_CFG_2 = 1<<2;
char DLPF_FS_SEL_0 = 1<<3;
char DLPF_FS_SEL_1 = 1<<4;

//I2C devices each have an address. The address is defined in the datasheet for the device. The ITG-3200 breakout board can have different address depending on how
//the jumper on top of the board is configured. By default, the jumper is connected to the VDD pin. When the jumper is connected to the VDD pin the I2C address
//is 0x69.
char GYRO_ADDRESS = 0x68;

unsigned char sendGyro[2] = {0, 0};
unsigned char RxBufferGyro[6];
int gyroX=0, gyroY=0, gyroZ=0;

void setup() {
  
  /* Port Mapping for I2C */
  UCB0CTL1 |= UCSWRST;
  PMAPPWD = 0x02D52;	// Get write-access to port mapping regs
  PMAPCTL = PMAPRECFG;  // Allow future runtime port mapping
  P2MAP0 = PM_UCB0SCL;	// Map SCL input to P2.0 
  P2MAP1 = PM_UCB0SDA;	// Map SDA output to P2.1 
  PMAPPWD = 0;		// Lock port mapping registers
  P2SEL |= BIT0 | BIT1;	//Set P2.0 and P2.1 to USCI Mode
  
  Serial.begin(9600);
  
  delay(1000);
  
  sendGyro[0] = SMPLRT_DIV;
  sendGyro[1] = 9;
  TI_USCI_I2C_transmitinit(GYRO_ADDRESS,12);
  TI_USCI_I2C_transmit(2,sendGyro);
  
  sendGyro[0] = DLPF_FS;
  sendGyro[1] = DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0;
  TI_USCI_I2C_transmitinit(GYRO_ADDRESS,12);
  TI_USCI_I2C_transmit(2,sendGyro);
  
  delay(70);
}

void loop() {
  
  sendGyro[0] = GYRO_XOUT_H;													//point to first gyro data register
  TI_USCI_I2C_transmitinit(GYRO_ADDRESS, 12);
  TI_USCI_I2C_transmit(1, sendGyro);
  TI_USCI_I2C_receiveinit(GYRO_ADDRESS, 12);
  TI_USCI_I2C_receive(6, RxBufferGyro);
  
  gyroX = ((int)RxBufferGyro[0]<<8) + (int)RxBufferGyro[1];
  gyroY = ((int)RxBufferGyro[2]<<8) + (int)RxBufferGyro[3];
  gyroZ = ((int)RxBufferGyro[4]<<8) + (int)RxBufferGyro[5];
  
  Serial.print("X: ");
  Serial.print(gyroX);
  Serial.print("  Y: ");
  Serial.print(gyroY);
  Serial.print("  Z: ");
  Serial.println(gyroZ);
  
  delay(100);
  
}
