#include <SpriteMag.h>

SpriteMag mag = SpriteMag();

float declination = 5*(PI/180);

void setup() {
  mag.init();
  Serial.begin(9600);
}

void loop() {
  
  MagneticField b = mag.read();
  
  //Calculate heading in radians from magnetic field components
  float heading = atan2(-b.y, -b.x);
  
  //correct for declination
  heading += declination;
  
  //Scale between 0 and 2*PI
  if(heading < 0) heading += 2*PI;
  if(heading > 2*PI) heading -= 2*PI;
  
  //Convert to degrees
  heading = heading*(180/PI);
  
  Serial.println(heading);
  
  Serial.print("x: ");
  Serial.print(b.x);
  Serial.print("    y:");
  Serial.print(b.y);
  Serial.print("    z: ");
  Serial.println(b.z);
  
  delay(500);
}
