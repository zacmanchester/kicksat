#ifndef HMC5883L_CONFIG_H_
#define HMC5883L_CONFIG_H_

/* This header contains all of the various configurations for the hmc5883l magnetometer
 * Select a configuration by uncommenting the appropriate #define(s).
 */

/*----------------------GAIN------------------------------------*/
/*Choose the magnetometer gain, only uncomment one.*/

#define MAG_GAIN 0b00000000 //0.73mG/LSb Range = +-0.88Ga
//#define MAG_GAIN 0b00100000 // 0.92mG/LSb Range = +-1.3Ga
//#define MAG_GAIN 0x01000000 // 1.22mG/LSb Range = +-1.9Ga
//#define MAG_GAIN 0x01100000 // 1.52mG/LSb Range = +-2.5Ga
//#define MAG_GAIN 0x10000000 // 2.27mG/LSb Range = +-4.0Ga
//#define MAG_GAIN 0x10100000 // 2.56mG/LSb Range = +-4.7Ga
//#define MAG_GAIN 0x11000000 // 3.03mG/LSb Range = +-5.6Ga
//#define MAG_GAIN 0x11100000 // 4.35mG/LSb Range = +-8.1Ga


/*-----------------SAMPLES AVERAGED-----------------------
 * choose how many samples are averaged per data reading, only uncomment one
 */
#define MAG_SAMPLES_AVE 0b00000000 //1 sample --default
//#define MAG_SAMPLES_AVE 0b00100000 //2 samples
//#define MAG_SAMPLES_AVE 0b01000000 //4 samples
//#define MAG_SAMPLES_AVE 0b01100000 //8 samples

/*-----------------DATA OUTPUT RATE-----------------------
 * Choose the data output rate in Hz, only uncomment one
 */
//#define MAG_DATA_RATE 0b00000000 //0.75Hz
//#define MAG_DATA_RATE 0b00000100 //1.5Hz
//#define MAG_DATA_RATE 0b00001000 //3Hz
//#define MAG_DATA_RATE 0b00001100 //7.5Hz
#define MAG_DATA_RATE 0b00010000 //15Hz -- default
//#define MAG_DATA_RATE 0b00010100 //30Hz
//#define MAG_DATA_RATE 0b00011000 //75Hz

/*-----------------BIAS-----------------------
 * Just use norma measurement, no bias
 */
#define MAG_MEAS_MODE 0b00000000

/*----------------OPERATING MODE-----------------------
 * Use continuous measurement for now
 */
#define MAG_OPER_MODE 0b00000000 //continuous measurement mode --default
//#define MAG_OPER_MODE 0b00000001 //single measurement mode (DON'T USE: CURRENTLY NOT SUPPORTED)
//#define MAG_OPER_MODE 0b00000010 //idle mode


#endif
