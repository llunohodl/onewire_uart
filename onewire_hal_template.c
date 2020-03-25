#include "stm8l15x.h"

/* HAL for 1-Wire UART communications */

/* Set UART baundrate to 9600*/
void OW_HAL_Speed_9600(){

}

/* Set UART baundrate to 115200*/
void OW_HAL_Speed_115200(){

}

/* Transmit and then receive byte thruout UART */
uint8_t OW_HAL_SendRecive(uint8_t data){
  return data;
}

/* Set 1-Wire pin to communication mode */
void OW_HAL_toUART(){

}

/* Set 1-Wire pin to powering device mode */
void OW_HAL_toPower(){

}