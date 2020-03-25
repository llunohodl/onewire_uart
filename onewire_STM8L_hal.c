#include "stm8l15x.h"

/* HAL for 1-Wire UART communications */

#define LP_MODE 1 //Low power: 1 - Core off (WFI mode) while TX performed 
//UART1_TX pin UART_PORT pin 3

#define LP_DEBUG 1 //Use PC2 pin for debug

#define UART_PORT GPIOC
#define UART_PIN  (1<<3)
#define DEBUG_PIN (1<<2)

/* Set UART baundrate to 9600*/
void OW_HAL_Speed_9600(){
  UART_PORT->ODR|=UART_PIN;
  UART_PORT->DDR|=UART_PIN;
  #if LP_DEBUG>0
  UART_PORT->ODR&=~DEBUG_PIN;
  UART_PORT->CR1|=DEBUG_PIN; //Push-pull
  UART_PORT->DDR|=DEBUG_PIN;
  #endif
  #if 0
  USART_Init(USART1,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,
             USART_Mode_Rx|USART_Mode_Tx);
  USART_HalfDuplexCmd(USART1, ENABLE);
  #else
  //9600
  USART1->BRR2=0x00;
  USART1->BRR1=0x1A;
  //TX/RX enable
  USART1->CR2=USART_CR2_TEN|USART_CR2_REN;
  //Half-duplex
  USART1->CR5=USART_CR5_HDSEL;
  #endif
}

/* Set UART baundrate to 115200*/
void OW_HAL_Speed_115200(){
  UART_PORT->ODR|=UART_PIN;
  UART_PORT->DDR|=UART_PIN;
  #if LP_DEBUG>0
  UART_PORT->ODR&=~DEBUG_PIN;
  UART_PORT->CR1|=DEBUG_PIN; //Push-pull
  UART_PORT->DDR|=DEBUG_PIN;
  #endif
  #if 0
  USART_Init(USART1,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,
             USART_Mode_Rx|USART_Mode_Tx);
  USART_HalfDuplexCmd(USART1, ENABLE);
  #else
  //115200
  USART1->BRR2=0x02;
  USART1->BRR1=0x02;
  //TX/RX enable
  USART1->CR2=USART_CR2_TEN|USART_CR2_REN;
  //Half-duplex
  USART1->CR5=USART_CR5_HDSEL;
  #endif
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
  #if LP_DEBUG>0
  UART_PORT->ODR|=DEBUG_PIN;
  #endif
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
    #if LP_DEBUG>0
    UART_PORT->ODR&=~DEBUG_PIN;
    #endif
    RX_data=USART1->DR;
    RX_flag=0;
    USART1->CR2&=~USART_CR2_RIEN;
  }
  #endif
}

/* Set 1-Wire pin to communication mode */
void OW_HAL_toUART(){
  UART_PORT->CR1&=~UART_PIN; //Pseudo open drain
  CLK->PCKENR1|=CLK_PCKENR1_USART1; //Power on UART
}

/* Set 1-Wire pin to powering device mode */
void OW_HAL_toPower(){
  CLK->PCKENR1&=~CLK_PCKENR1_USART1; //Power off UART
  UART_PORT->CR1|=UART_PIN; //Push-pull
}