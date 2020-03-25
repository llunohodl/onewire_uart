#include "DS18B20.h"
#include "onewire.h"

#define SKIP_ROM         0xCC
#define CONVERT_T        0x44
#define READ_SCRATCHPAD  0xBE
#define WRITE_SCRATCHPAD 0x4E

#define RESOLUTION_9BIT  0x1F
#define RESOLUTION_10BIT 0x3F
#define RESOLUTION_11BIT 0x5F
#define RESOLUTION_12BIT 0x7F

int8_t DS18B20_start(){
  uint8_t cmd[5]={SKIP_ROM,WRITE_SCRATCHPAD,0x64,0x9E,RESOLUTION_12BIT};
  return OW_Read_Poll(cmd,5,NULL,0);
}

int8_t DS18B20_convert_T(){
  uint8_t cmd[2]={SKIP_ROM,CONVERT_T};
  return OW_Read_Poll(cmd,2,NULL,0);
}

int8_t DS18B20_get_T(uint8_t* T_array){
  uint8_t cmd[4]={SKIP_ROM,READ_SCRATCHPAD,0xFF,0xFF};
  return OW_Read_Poll(cmd,4,T_array,2);
}