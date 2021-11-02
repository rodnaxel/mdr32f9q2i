#include "util.h"

//Функция записи состояния (1:0) светодиода VD7
void led_Write(bool on_off)
{
	PORT_WriteBit(MDR_PORTC, PORT_Pin_2, on_off ? SET  : RESET);		
}

//Функция считывания текущего состояния кнопки SA4
uint8_t button_State(void)
{
	return PORT_ReadInputDataBit(MDR_PORTD, PORT_Pin_5);
}
