/**********************************************************************************************
* Device driver software to interface STM32F103 MCU with MAX7219 (8x8 Dot Matrix display)
* Name: main.c
* Author: Manjula Narayanaswamy
* Created on: 22/08/2021
**********************************************************************************************/


#include "stm32f10x.h"

#include "MATRIX.h"


int main(){
	
	USART_Init();
	SPI_Init();
	MATRIX_Init();
	//SEG_Init();
	
	while(1){
		//SEG_Init();
		PRINT_Alphabets();
	}
	
}
