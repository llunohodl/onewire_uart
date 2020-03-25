#ifndef DS18B20_H_
#define DS18B20_H_

#ifndef uint8_t
#include <stdint.h>
#endif

int8_t DS18B20_start();
int8_t DS18B20_get_T(uint8_t* T_array);

#endif /* DS18B20_H_ */