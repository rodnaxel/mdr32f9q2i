#ifndef __UART_H__
#define __UART_H__

#include "global.h"

void uart_init(void);
uint8_t uart_Work(void);
uint8_t uart_Send(void);

uint8_t putByte(uint8_t byte);
uint8_t getByte(void);

#endif
