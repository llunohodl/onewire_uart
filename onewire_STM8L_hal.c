#include "stm8l15x.h"

/* HAL for 1-Wire UART communications */

#define LP_MODE 1 //Low power: 1 - Core off (WFI mode) while TX performed 
//UART1_TX pin GPIOC pin 3

/* Set UART baundrate to 9600*/
void OW_HAL_Speed_9600(){
  GPIOC->ODR|=(1<<3);
  GPIOC->DDR|=(1<<3);
  USART_Init(USART1,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,
             USART_Mode_Rx|USART_Mode_Tx);
  USART_HalfDuplexCmd(USART1, ENABLE);
}

/* Set UART baundrate to 115200*/
void OW_HAL_Speed_115200(){
  GPIOC->ODR|=(1<<3);
  GPIOC->DDR|=(1<<3);
  USART_Init(USART1,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,
             USART_Mode_Rx|USART_Mode_Tx);
  USART_HalfDuplexCmd(USART1, ENABLE);
}

static uint8_t RX_data=0;
#if LP_MODE>0
static uint8_t RX_flag=0;
#endif
/* Transmit and then receive byte thruout UART */
uint8_t OW_HAL_SendRecive(uint8_t data){
  uint8_t ret = USART1->DR;     //Reset RXNE
  USART1->SR&=~USART_SR_TC; //Reset TC
  USART1->DR = data; //Start transmit
#if LP_MODE>0
  USART1->CR2 |= USART_CR2_RIEN;
  RX_flag=1;
  do{
    wfi(); //Wait for interrupt
  }while(RX_flag);
#else
  while((USART1->SR&USART_SR_RXNE)==0);
  RX_data = USART1->DR;
#endif
  return RX_data;
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28){
  #if LP_MODE>0
  if(USART1->SR&USART_SR_RXNE){
    RX_data=USART1->DR;
    RX_flag=0;
    USART1->CR2&=~USART_CR2_RIEN;
  }
  #endif
}

/* Set 1-Wire pin to communication mode */
void OW_HAL_toUART(){
  GPIOC->CR1&=~(1<<3); //Pseudo open drain
  CLK->PCKENR1|=CLK_PCKENR1_USART1; //Power on UART
}

/* Set 1-Wire pin to powering device mode */
void OW_HAL_toPower(){
  CLK->PCKENR1&=~CLK_PCKENR1_USART1; //Power off UART
  GPIOC->CR1|=(1<<3); //Push-pull
}