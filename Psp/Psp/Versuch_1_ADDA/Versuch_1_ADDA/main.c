#include <avr/io.h>
#include <util/delay.h>
#include "os_input.h"
#include <stdio.h>
#include <stdbool.h>
#include "wandler.h"



void init_Wandler(){
DDRD &= 0b00000000;
PORTD |= 0b11111111;
DDRA |= 0b11111111;
DDRB |= 0b11111111;
}

int main(void) {
	init_Wandler();
	int8_t var = 1;
	switch(var){
		case 1: 
		manuell();
		case 2:
		sar();
		case 3:
		tracker();
	}
}

