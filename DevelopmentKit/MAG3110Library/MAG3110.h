#ifndef MAG3110_LIB
#define MAG3110_LIB

#define CTRL_REG1_ADDR  0x10
#define CTRL_REG2_ADDR  0x11

void MAG3110_initialize();
void MAG3110_read_once(unsigned int *store);

#endif
