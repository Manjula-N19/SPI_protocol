#include "stm32f10x.h"
#include "USART.h"
#include "stdio.h"
#include "stdlib.h"

#define SLAVE_SELECT   GPIOA->ODR &= ~GPIO_ODR_ODR4;
#define SLAVE_DESELECT GPIOA->ODR |= GPIO_ODR_ODR4;

//#define SLAVE_SELECT   GPIOA->CRL &= ~0x000F0000;
//#define SLAVE_DESELECT GPIOA->CRL |= 0x000F0000;

//#define SLAVE_SELECT SPI1->CR1 &= ~ SPI_CR1_SSI;
//#define SLAVE_DESELECT SPI1->CR1 |= SPI_CR1_SSI;


void SPI_Init(){
  delay_ms(10);
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN |    // Alternate function enable
									RCC_APB2ENR_IOPAEN |    // PORTA clock enable
									RCC_APB2ENR_SPI1EN;     // SPI1 enable
	
	GPIOA->CRL |= 0xF0F30000;  // Configure PA4 = SPI_NSS; PA5 = CLK; PA7 = MOSI, as outputs
	//SPI1->CR2 |= SPI_CR2_SSOE;  // NSS in output mode; Hardware NSS management; enable SSOE bit
	//SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI ;
	 //SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1; // Baud rate fpclk/16
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;  // MSB first
	SPI1->CR1 &= ~ SPI_CR1_DFF; // DFF = 0, 8-bit format
	SPI1->CR1 |=  SPI_CR1_MSTR | SPI_CR1_SPE ; // Master mode enable; SPI enable
	
	delay_ms(10);

}

uint8_t alpha_table[208] = {    
		
		0x3E, 0x40, 0x40, 0x7E, 0x02, 0x02, 0x7C, 0x00, //S
		0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00,  //O		 
		0x42, 0x62, 0x52, 0x4A, 0x46, 0x42, 0x42, 0x00, //N
		0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, // U 		

};



void WRITE_byte(uint8_t byte){
	if(SPI1->SR & SPI_SR_TXE) 
	SPI1->DR = byte;
	while(SPI1->SR & SPI_SR_BSY)
	;  // Wait until the data is transmitted and SPI flag is set.
}

void WRITE_word(uint16_t word){
	if(SPI1->SR & SPI_SR_TXE)
	SPI1->DR  = word>>8;  // the highest byte - address
	//USART_Tx_string("highest byte transmitted");
	while(SPI1->SR & SPI_SR_BSY)
	;
	SPI1->DR = word; // the lowest byte - data
	//USART_Tx_string("lowest byte transmitted");
	while(SPI1->SR & SPI_SR_BSY)
	;
}

void MATRIX_Init(){
	
	SLAVE_SELECT;
	//USART_Tx_string("Slave select - set brightness");
	WRITE_byte(0x0A);    // select intensity register
	WRITE_byte(0x07);    // set brightness
	SLAVE_DESELECT;
	delay_ms(100);
	
	SLAVE_SELECT;
	//USART_Tx_string("Slave select - set scan limit register");
	WRITE_byte(0x0B);    // select scan limit register
	WRITE_byte(0x07);    // select 0-7 rows
	SLAVE_DESELECT;
	delay_ms(100);
	
	SLAVE_SELECT;
	//USART_Tx_string("Slave select - set normal mode of operation");
	WRITE_byte(0x0C);    // select shut down register
	WRITE_byte(0x01);    // select normal mode of operation 
	SLAVE_DESELECT;
	delay_ms(100);
	
	SLAVE_SELECT;
	//USART_Tx_string("Slave select - Enable display test");
	WRITE_byte(0x0F);    // select Display-test regist er
	WRITE_byte(0x01);    // enable Display test
	SLAVE_DESELECT;
  delay_ms(100);	
	
}

  
void SEG_Init(){
  SLAVE_SELECT;
	WRITE_byte(0x09);
	WRITE_byte(0xFF);	//enable decoding for all digits 
	SLAVE_DESELECT;
  delay_ms(100);	
	
	SLAVE_SELECT;
	WRITE_byte(0x0B);
	WRITE_byte(0x01);		/* 2 (1+1) digits */
	SLAVE_DESELECT;
  delay_ms(100);	
	
	SLAVE_SELECT;
	WRITE_byte(0x0C);
	WRITE_byte(0x01);	/* turn on */
	SLAVE_DESELECT;
  delay_ms(100);
	
	SLAVE_SELECT;
	WRITE_byte(0x0A);
	WRITE_byte(0x07);	/* show 4 on digit 2 */	
	SLAVE_DESELECT;
  delay_ms(100);
	
	SLAVE_SELECT;
  WRITE_byte(0x01);
	WRITE_byte(0x04);	/* show 9 on digit 1 */
	SLAVE_DESELECT;
  delay_ms(100);
}

void PRINT_Alphabets(){
	
	
	uint8_t s=0;
	uint16_t data;
	uint8_t l, row;
	
	for(l = 0; l<5; l++){    // To print 4 letters
		//USART_Tx_string("Printing letters");
		for(row = 1; row<9; row++){    //8 rows of matrix
			
			//USART_Tx_string("row 1 ");
			SLAVE_SELECT;
			//USART_Tx_string("row 1 slave select");
			data = ((row<<8) | alpha_table[s]);
			WRITE_word(data);
			SLAVE_DESELECT;
			s++;
			delay_ms(10);		
		}
	delay_ms(50);
		
	}

}
