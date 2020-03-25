#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#ifndef uint8_t
#include <stdint.h>
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

/* OW_Read_Poll send command polling
 * Tx - transmit data array / TxLen transmit data array len
 * Rx - receive data array / RxLen receive data array len 
 * RxLen<=TxLen reading only last bytes, for example:
 * TxLen = 8 / RxLen = 6 in Rx data recived when bytes 2..8 transmitted
 */
int8_t OW_Read_Poll(uint8_t* Tx, uint8_t TxLen, 
                    uint8_t* Rx, uint8_t RxLen);

#endif /* ONEWIRE_H_ */
