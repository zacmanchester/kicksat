#ifndef ITG3200_CONFIG_H_
#define ITG3200_CONFIG_H_

/* This header contains all of the various configurations for the itg3200 gyro
 * Select a configuration by uncommenting the appropriate #define(s).
 */

#define SMPL_RATE_REG_ADDR  0x15
#define DLPF_RANGE_REG_ADDR 0x16

/*----------------------ADDRESS------------------------------------*/
/*Choose the gyro i2c address (depending on whether if VIO is high or low)
 */

#define GYRO_ADDRESS 0x69 //VIO set to HIGH --default
//define GYRO_ADDRESS 0x68 //VIO set to LOW


/*-----------------SAMPLES RATE DIVIDER-----------------------
 * Choose the sample rate of the itg3200 gyro. The outpuits are sampled internally
 * at either 1kHz or 8kHz. This sampling is filtered and delivered to the sensor registers
 * after the number of cycles determined by this register. The sample rate is given by:
 *
 * F_sample = F_interal/(divider+1) where F_internal is either 1kHz or 8kHz
 *
 * Example: F_sample = 1kHz/(7+1) = 125Hz, or 8ms per sample
 *
 * Change GYRO_SAMPLE_RATE to a decimal number of your choosing. Defualt is 7
 * leading to the sample rate in the example above, 125Hz.
 */
#define GYRO_SAMPLE_RATE 7 // --default = 7



/*-----------------RANGE----------------------
 * Only one option for gyro range: +-2000 deg/sec.
 */
#define GYRO_RANGE 0b00011000 //+-2000 deg/sec, only available option



/*-----------------SENSITIVITY SCALE FACTOR----------------------
* Nothing to set, just information about the sensitivity scale factors for
* gyro readings and temperature readings.
*
* GYRO SENSITIVITY SCALE FACTOR = 14.375 LSb / (deg/sec)      +-6% accuracy at 25 degrees Celsius
* 															  +-10% variation over temperature
*
* TEMPERATURE SENSITIVITY SCALE FACTOR = 280 LSb/ degree celsius
* -range = -30 to +85 degrees celsius
* -initial offset = -13000 LSb = 35 degrees celsius





/*-----------------DIGITAL LOW PASS FILTER CONFIGURATION (DLPF) AND INTERNAL SAMPLE RATE------
 * Uncomment one to specifiy the low pass filter configuration and internal
 * sampling rate.
 */
//											  LOW PASS FILTER BANDWIDTH | INTERNAL SAMPLE RATE
//#define GYRO_FILTER_SMPL_RATE 0b00000000; //			256 Hz			|		8kHz
//#define GYRO_FILTER_SMPL_RATE 0b00000001; //			188 Hz			|		1kHz
//#define GYRO_FILTER_SMPL_RATE 0b00000010; //			 98 Hz			|		1kHz
//#define GYRO_FILTER_SMPL_RATE 0b00000011; //			 42 Hz			|		1kHz
//#define GYRO_FILTER_SMPL_RATE 0b00000100; //			 20 Hz			|		1kHz
//#define GYRO_FILTER_SMPL_RATE 0b00000101; //			 10 Hz			|		1kHz
#define GYRO_FILTER_SMPL_RATE 0b00000110; //			  5 Hz			|		1kHz    --Default



#endif
