#ifndef GYRO_LIB
#define GYRO_LIB

void ITG3200_initialize();
void ITG3200_read_once(unsigned int *store);
void ITG3200_read_temp(unsigned int *tempStore);

#endif
