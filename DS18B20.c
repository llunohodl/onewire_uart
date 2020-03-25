#include "DS18B20.h"
#include "onewire.h"

#define SKIP_ROM         0xCC

#define RESOLUTION_9BIT  0x1F
#define RESOLUTION_10BIT 0x3F
#define RESOLUTION_11BIT 0x5F
#define RESOLUTION_12BIT 0x7F

int8_t DS18B20_start(){
  uint8_t dat[]={SKIP_ROM,0x4E,0x64,RESOLUTION_12BIT};
  OW_Read_Poll(dat,4,NULL,0);
}

int8_t DS18B20_get_T(uint8_t* T_array){

}