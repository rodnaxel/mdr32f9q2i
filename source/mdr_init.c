#include "mdr_init.h"


void device_init() {
	rst_clock_init();
	sysTimer_init();
	ports_init();
}


static void ports_init(void) {
	//Создание структуры для инициализации порта
	PORT_InitTypeDef PORT_InitStructure;

	//Настройки порта: вывод, функция ввода/вывода, цифровой режим, максимальная скорость, Pin2
	PORT_InitStructure.PORT_OE = PORT_OE_OUT;
	PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
	PORT_InitStructure.PORT_Pin = (PORT_Pin_2);	
	PORT_Init(MDR_PORTC, &PORT_InitStructure);
	
	//Настройки порта: ввод, функция ввода/вывода, цифровой режим, минимальная скорость, Pin5
	PORT_InitStructure.PORT_OE = PORT_OE_IN;
	PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_InitStructure.PORT_Pin = (PORT_Pin_5); 	
	PORT_Init(MDR_PORTD, &PORT_InitStructure);	
}


#ifndef CMSIS_ENABLE
//Функция настройки тактовой частоты МК
static void rst_clock_init(void) {
	//Включение тактирования от внешнего источника HSE (High Speed External) 
	RST_CLK_DeInit();

	//Включение тактирования от внешнего источника HSE (High Speed External)
	RST_CLK_HSEconfig(RST_CLK_HSE_ON);
	if (RST_CLK_HSEstatus() == ERROR) { 
		while (1) {};
	}  	

	// Включение задержки EEPROM для работы на частоте до 100МГц
	// Выполняется до блока PLL
	EEPROM_SetLatency(EEPROM_Latency_3);
	
	RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv2, RST_CLK_CPU_PLLmul10);

	//Включение CPU_PLL
	RST_CLK_CPU_PLLcmd(ENABLE);
	if (RST_CLK_CPU_PLLstatus() == ERROR) {
		while (1) {};
	};  
	RST_CLK_CPU_PLLuse(ENABLE);

	// Выбор источника тактирования ядра процессора 
	RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
	
	// Обновление переменной SystemCoreClock, хранящую текущую частоту тактирования
	SystemCoreClockUpdate();

	// Подача тактовой частоты на порты PORTD, PORTC, PORTF
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF, ENABLE);
	
	// Подача тактовой частоты на UART2
	RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);
}

static void sysTimer_init (void){
	SysTick_Config(SystemCoreClock/1000 - 1);
	NVIC_EnableIRQ(SysTick_IRQn);
}

#else 

static void rst_clock_init(void) {
	// Включение тактирования от внешнего источника HSE (High Speed External)
	MDR_RST_CLK->HS_CONTROL |= RST_CLK_HS_CONTROL_HSE_ON;
	while(!(MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY)) {};

	// Источник для CPU_C1 HSE/2(0x03) 
	MDR_RST_CLK->CPU_CLOCK |= 0x03 << RST_CLK_CPU_CLOCK_CPU_C1_SEL_Pos;

	// Включение задержки EEPROM для работы на частоте до 100МГц (выполняется до блока PLL)
	MDR_EEPROM->CMD |= 0x04 << EEPROM_CMD_DELAY_Pos;

	// Поднимаем тактовую частоту в n=10 раз
	MDR_RST_CLK->PLL_CONTROL |= (10 - 1) << RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos;
	MDR_RST_CLK->PLL_CONTROL |= RST_CLK_PLL_CONTROL_PLL_CPU_ON;
	while (!(MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY))

	// Источник для CPU_C2
	MDR_RST_CLK->CPU_CLOCK |= 1 << RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos;
	
	// Выбор источника тактирования HCLK (CPU_C3)
	MDR_RST_CLK->CPU_CLOCK |= 1 << RST_CLK_CPU_CLOCK_HCLK_SEL_Pos;
	
	// Обновление переменной SystemCoreClock, хранящую текущую частоту тактирования
	SystemCoreClockUpdate();
	
	// Подача тактовой частоты на порты PORTD, PORTC, PORTF
	MDR_RST_CLK->PER_CLOCK |= (1 << 23) | (1 << 24) | (1 << 25);
	
	// Подача тактовой частоты на UART2
	MDR_RST_CLK->PER_CLOCK |= (1 << 7);
}

static void sysTimer_init() {
	SysTick->LOAD = (SystemCoreClock / 1000) - 1; 
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	NVIC_EnableIRQ(SysTick_IRQn);
}
#endif
