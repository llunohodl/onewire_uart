#include "stm8l15x.h"

/* HAL for 1-Wire UART communications */

#define LP_MODE 1 //Low power: 1 - Core off (WFI mode) while TX performed 
//UART1_TX pin UART_PORT pin 3
#ifdef STM8L15X_MD
#define UART_PORT GPIOC
#define UART_PIN  (1<<3)
#else //STM8L15X_LD
#define UART_PORT GPIOC
#define UART_PIN  (1<<5)
#endif

void DBG_On();
void DBG_Off();

#define POWER_PORT GPIOC
#define POWER_PIN  (1<<6)

/* Set UART baundrate to 9600*/
void OW_HAL_Speed_9600(){
  UART_PORT->ODR|=UART_PIN;
  UART_PORT->DDR|=UART_PIN;
  #if 0
  USART_Init(USART1,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,
             USART_Mode_Rx|USART_Mode_Tx);
  USART_HalfDuplexCmd(USART1, ENABLE);
  #else
  #ifdef STM8L15X_LD
  //USART1_TX on PC5 and USART1_RX on PC6 
  SYSCFG->RMPCR1|=(2<<4);
  #endif
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
  USART1->CR2 |= USART_CR2_RIEN|USART_CR2_TCIEN;
  RX_flag=1;
  DBG_Off();
  wfi(); //Wait for interrupt
  DBG_On();
  if(RX_flag==1){
    RX_data=data;
  }
  USART1->CR2 &=~(USART_CR2_RIEN|USART_CR2_TCIEN);
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

INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27){
    USART1->CR2&=~USART_CR2_TCIEN;
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

/* Off 1-Wire sensor*/
void OW_PowerOff(){
  UART_PORT->ODR&=~UART_PIN;
  UART_PORT->DDR|=UART_PIN;
  POWER_PORT->ODR&=~POWER_PIN;
  POWER_PORT->DDR|=POWER_PIN;
  POWER_PORT->CR1|=POWER_PIN;
}

/* On 1-Wire sensor*/
void OW_PowerOn(){
  UART_PORT->ODR|=UART_PIN;
  UART_PORT->DDR|=UART_PIN;
  POWER_PORT->ODR|=POWER_PIN;
  POWER_PORT->DDR|=POWER_PIN;
  POWER_PORT->CR1|=POWER_PIN;
}