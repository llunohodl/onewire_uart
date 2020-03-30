#include "onewire.h"




#define OW_0	0x00
#define OW_1	0xff

/* OW_toBits convert byte to bit array
 * ow_byte - input byte
 * ow_bits - out byte array LSB first
 */
void OW_toBits(uint8_t ow_byte, uint8_t *ow_bits) {
  for (uint8_t mask=0x01; mask>0; mask<<=1) {
    *ow_bits=(ow_byte & mask) ? OW_1 : OW_0;
    ow_bits++;
  }
}

#define OW_toBit(byte,bit) (((byte)&(1<<(bit))) ? OW_1 : OW_0)

/* OW_toByte bit array to convert byte static MACRO
 * ow_bits - in byte array LSB first
 * return - out byte 
 */
#define OW_toBits_static(byte) \
OW_toBit(byte,0), OW_toBit(byte,1), OW_toBit(byte,2), OW_toBit(byte,3),\
OW_toBit(byte,4), OW_toBit(byte,5), OW_toBit(byte,6), OW_toBit(byte,7),

/* OW_toByte bit array to convert byte
 * ow_bits - in byte array LSB first
 * return - out byte 
 */
uint8_t OW_toByte(uint8_t *ow_bits){
  uint8_t ret=0;
  for (uint8_t mask=0x01; mask>0; mask<<=1) {
    ret|= (*ow_bits==OW_1) ? mask : 0;
    ow_bits++;
  }
  return ret;
}

/* OW_Reset check devs on line
 * return 0 - found dev
 */
static int8_t OW_Reset() {
	OW_HAL_Speed_9600();
    uint8_t ret = OW_HAL_SendRecive(0xf0);
    OW_HAL_Speed_115200();
    return ret == 0xF0 ? -1 : 0;
}

/* OW_Read_Poll send command polling
 * Tx - transmit data array / TxLen transmit data array len
 * Rx - receive data array / RxLen receive data array len 
 * RxLen<=TxLen reading only last bytes, for example:
 * TxLen = 8 / RxLen = 6 in Rx data recived when bytes 2..8 transmitted
 */
int8_t OW_Read_Poll(uint8_t* Tx, uint8_t TxLen, 
                    uint8_t* Rx, uint8_t RxLen){
  
  OW_HAL_toUART();

  if(OW_Reset()!=0){
    OW_HAL_toPower();
    return -1;
  }
  
  while(TxLen){
    if(TxLen>RxLen){
      for(uint8_t mask=0x01; mask>0; mask<<=1){
        //Send without receive
        OW_HAL_SendRecive( mask&(*Tx) ? OW_1 : OW_0);
      }
    }else{
      *Rx=0; //Reset all bits
      for(uint8_t mask=0x01; mask>0; mask<<=1){
        //Send with receive
        if(OW_HAL_SendRecive( mask&(*Tx) ? OW_1 : OW_0)==OW_1){
          *Rx|= mask; //set bit
        }
      }
      Rx++;
    }
    Tx++;
    TxLen--;
  }
  OW_HAL_toPower();
  return 0;
}
