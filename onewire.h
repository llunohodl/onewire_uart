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
/* HAL functions prototypes for 1-Wire UART communications */

/* Set UART baundrate to 9600*/
void OW_HAL_Speed_9600();
/* Set UART baundrate to 115200*/
void OW_HAL_Speed_115200();
/* Transmit and then receive byte thruout UART */
uint8_t OW_HAL_SendRecive(uint8_t data);
/* Set 1-Wire pin to communication mode */
void OW_HAL_toUART();
/* Set 1-Wire pin to powering device mode */
void OW_HAL_toPower();
/* Off 1-Wire sensor*/
void OW_PowerOff();
/* On 1-Wire sensor*/
void OW_PowerOn();

#endif /* ONEWIRE_H_ */
