#include <Wire.h>
#include <HMC5883L.h>

HMC5883L compass;

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
  
  Wire.begin();
  
  Serial.println("Hi");
  compass = HMC5883L(); //new instance of HMC5883L library
  setupHMC5883L(); //setup the HMC5883L
}

// Our main program loop.
void loop(){
  
  float heading = getHeading();
  Serial.println(heading);
  delay(500);
}

void setupHMC5883L(){
  //Setup the HMC5883L, and check for errors
  int error = 0;  
  error = compass.SetScale(1.3); //Set the scale of the compass.
  Serial.println(error);
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  Serial.println(error);
}

float getHeading(){
  //Get the reading from the HMC5883L and calculate the heading
  MagnetometerScaled scaled = compass.ReadScaledAxis(); //scaled values from compass.
  float heading = atan2(scaled.YAxis, scaled.XAxis);

  // Correct for when signs are reversed.
  if(heading < 0) heading += 2*PI;
  if(heading > 2*PI) heading -= 2*PI;

  return heading * RAD_TO_DEG; //radians to degrees
}
