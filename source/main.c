#include "clk.h"
#include "led.h"
#include "uart.h"
#include "button.h"

char b_sendByte;

char putChar(char byte) {
	UART_SendData(MDR_UART2, byte);
	return 1;
}


int main(void)
{

	//Переменная для чтения команды по UART
	uint8_t state = 0;
	
	//Инициализация периферии
	clk_CoreConfig();
	led_Init();
	button_Init();
	uart_Init();
	
	led_Write(1);

	
  while (1){
		state = button_State();
		
		led_Write(!state);
		
		if (state) {
			if (!b_sendByte) { 
				putChar(1);
				b_sendByte = 1;
			}
		
		} else {
			b_sendByte = 0;
		}
	}
}
