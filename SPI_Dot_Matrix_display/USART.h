#include "stm32f10x.h"


void USART_Init(){   // Initialise serial USART
	RCC->APB2ENR |= 0x00000005; // Enable AFIO - Alternate functions of GPIO and IOPA clock enable
	RCC->APB1ENR |= 0x0020000; // Enable USART2 - bit 17
	GPIOA->CRL |= 0x00B400;   //PA2 = TX - make as input; PA3 = RX - make as output
	USART2->BRR = 0x7530;  //2400 baud rate at 72MHz
	USART2->CR1 |= 0x000200C; // Receiver enable
}

char USART_read(){  // receive character byte
	while(!(USART2->SR & 0x0020)) // check if Rx flag is set
		;  // Do Nothing
	return USART2->DR;
}

void USART_write(char ch){  // send one byte data
	while(!(USART2->SR & 0x0080))  // wait until Tx buffer is empty 
		;
	USART2->DR = ch;
}

void delay_ms(int delay){  // psuedo-delay function
	volatile int i;
	for(;delay>0; delay--)
	for(i=0; i<5000; i++);
}
	
void USART_Tx_string(char text[]){
	int i=0;
	while(i<255){
		if (text[i] == '\0')
		break;
		USART_write(text[i]);		
		i++;
	}
}
