#include "clk.h"
#include "led.h"
#include "uart.h"
#include "button.h"
#include "mdr_init.h"

char b_sendByte;

uint8_t b_led_switch;
uint16_t counter_led = 500;

uint8_t b_message_sent;
uint16_t counter_message = 1000;


void SysTick_Handler() {
	
	counter_led--;
	if (!counter_led) {
		b_led_switch = 1;
		counter_led = 500;
	}
	
	counter_message--; 
	if (!counter_message) {
		b_message_sent = 1;
		counter_message = 1000;
	}

}

void blink_led() {
	static bool state = 0;
	if (b_led_switch) {
			state = !state;
			led_Write(state);
			b_led_switch = 0;
		}
}

void send_message() {
	if (b_message_sent) {
		putByte(0x01);
		b_message_sent = 0;
	}
}

int main(void) {

	mdr_init();
	
	//Инициализация периферии
	uart_init();
	led_Init();
	button_Init();	
	led_Write(1);
	
  while (1){
		blink_led();
		send_message();
	}
}
