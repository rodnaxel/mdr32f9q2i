#include "uart.h"

//Функция инициализации UART2
void uart_init()
{
	//Создание структур для инициализации выводов порта и UART
	PORT_InitTypeDef PortInit;
	UART_InitTypeDef UART_InitStructure;
	
	//Настройка порта: основная функция, цифровой режим, максимальная скорость
	//Отключение: подтягивающих резисторов, функций триггера шмидта, филтрации Гаусса
	PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
	PortInit.PORT_PD = PORT_PD_DRIVER;
	PortInit.PORT_GFEN = PORT_GFEN_OFF;
	PortInit.PORT_FUNC = PORT_FUNC_OVERRID;
	PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
	PortInit.PORT_MODE = PORT_MODE_DIGITAL;

	//Настойка PORTF pins 1 на вывод (UART2_TX)
	PortInit.PORT_OE = PORT_OE_OUT;
	PortInit.PORT_Pin = PORT_Pin_1;
	PORT_Init(MDR_PORTF, &PortInit);

	//Настойка PORTF pins 0 на ввод (UART2_RX)
	PortInit.PORT_OE = PORT_OE_IN;
	PortInit.PORT_Pin = PORT_Pin_0;
	PORT_Init(MDR_PORTF, &PortInit);
	
	//Установка предделителя часототы UART2 HCLKdiv = 16
	UART_BRGInit(MDR_UART2, UART_HCLKdiv16 );

	//Настройка параметров UART2: 115200, 8бит, 1 стоп бит, без паритета, откл. буфера FIFO, контроль потока RX/TX
	UART_InitStructure.UART_BaudRate = 9600;
	UART_InitStructure.UART_WordLength = UART_WordLength8b;
	UART_InitStructure.UART_StopBits = UART_StopBits1;
	UART_InitStructure.UART_Parity = UART_Parity_No;
	UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
	UART_InitStructure.UART_HardwareFlowControl =  UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

	//Конфигурирование параметров UART2
	UART_Init(MDR_UART2, &UART_InitStructure);

	//Команда включения UART2
	UART_Cmd(MDR_UART2, ENABLE);
}


//Функция приема байта по UART2
uint8_t uart_Work(void)
{
	uint16_t data;
	uint8_t res = 0xFF;

	if(UART_GetFlagStatus(MDR_UART2, UART_FLAG_RXFE) != SET) 
	{
			
		data = UART_ReceiveData(MDR_UART2);

		if((char)(data & 0xFF) == '0')
			res = 0x00;
		else if((char)(data & 0xFF) == '1')
			res = 0x01;
	}
	
	return res;
}
