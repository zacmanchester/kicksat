// ITG-3200_test
// Copyright 2010-2011 Filipe Vieira & various contributors.
// http://code.google.com/p/itg-3200driver
// Simple test of gyro sensors output using default settings.

#include <Wire.h>
#include <ITG3200.h>

ITG3200 gyro = ITG3200();
float  x,y,z;
int ix, iy, iz;

void setup(void) {
  
  /* Port Mapping for I2C */
  UCB0CTL1 |= UCSWRST;
  PMAPPWD = 0x02D52;	// Get write-access to port mapping regs
  PMAPCTL = PMAPRECFG;  // Allow future runtime port mapping
  P2MAP0 = PM_UCB0SCL;	// Map SCL input to P2.0 
  P2MAP1 = PM_UCB0SDA;	// Map SDA output to P2.1 
  PMAPPWD = 0;		// Lock port mapping registers
  P2SEL |= BIT0 | BIT1;	//Set P1.1 and P1.2 to USCI Mode
  
  Wire.begin();      // if experiencing gyro problems/crashes while reading XYZ values
                     // please read class constructor comments for further info.
  
  Serial.begin(9600);
  
  Serial.println("Serial port initialized");
  
  delay(1000);
  // Use ITG3200_ADDR_AD0_HIGH or ITG3200_ADDR_AD0_LOW as the ITG3200 address 
  // depending on how AD0 is connected on your breakout board, check its schematics for details
  gyro.init(ITG3200_ADDR_AD0_HIGH); 
  
  Serial.print("zeroCalibrating...");
  gyro.zeroCalibrate(2500, 2);
  Serial.println("done.");
}

void loop(void) {
    while (gyro.isRawDataReady()) {
    /* 
    // Reads uncalibrated raw values from the sensor 
    gyro.readGyroRaw(&ix,&iy,&iz); 
    Serial.print("X1:"); 
    Serial.print(ix); 
    Serial.print("  Y:"); 
    Serial.print(iy); 
    Serial.print("  Z:"); 
    Serial.println(iz); 
    */ 
     
    /* 
    // Reads calibrated raw values from the sensor 
    gyro.readGyroRawCal(&ix,&iy,&iz); 
    Serial.print("X2:"); 
    Serial.print(ix); 
    Serial.print("  Y:"); 
    Serial.print(iy); 
    Serial.print("  Z:"); 
    Serial.println(iz); 
    */ 
     
    // Reads calibrated values in deg/sec    
    gyro.readGyro(&x,&y,&z); 
    Serial.print("X3:"); 
    Serial.print(x); 
    Serial.print("  Y:"); 
    Serial.print(y); 
    Serial.print("  Z:"); 
    Serial.println(z);
  } 
}



