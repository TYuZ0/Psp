#include <avr/io.h>
#include <util/delay.h>
#include "os_input.h"
#include <stdio.h>
#include <stdbool.h>
#include "wandler.h"

void manuell(void){
	DDRD &= 0b00000000;
	PORTD |= 0b11111111;
	DDRA |= 0b11111111;
	DDRB |= 0b11111111; //Register gesetzt
	uint8_t eingabe = 0b00000000;
	while(1){ //wie funktioniert kontinuierliche Übertragung?
		eingabe |= ~PIND ; // 0 = ON |ließt Eingabe
		PORTA &= 0b00000000;
		PORTB &= 0b00000000;
		PORTB |= eingabe; // Eingabe in B UND A als Ausgabe eingeben
		PORTA |= eingabe;
		
	}
}
void sar(){
	
	uint8_t ref = 0b00000000;
	while(1){
		os_waitForInput();
		_delay_ms(50);
		PORTB &= 0b00000000;
		PORTA &= 0b00000000;
		// Spezifikationen von Wandler
		
		
		for(int i=7; i>=0; i--){
			ref |= (1<<i);
			PORTB &= 0b00000000;
			PORTB |= ref;
			PORTA &= 0b00000000;
			PORTA |= ref;
			_delay_ms(50);
			if((PORTC &= 0b00000001 ) == 0b00000001 ){
				ref &= ~(1 << i);
			}
			else {
				ref |= (1 << i);
			}
		}
		PORTB &= 0b00000000;
		PORTB |= ref;
		PORTA &= 0b00000000;
		PORTA |= ref;
		
	}
}
void tracker(){
	uint8_t ref = 0b10000000;
	bool momentan = false;
	bool anfangs = false;
	
	while(1){
		os_waitForInput(); // c1 muss gedrückt sein
		PORTA &= 0b00000000;
		PORTA |= ref;
		PORTB &= 0b00000000;
		PORTB |= ref;
		_delay_ms(50);
		momentan = (PORTC &= 0b00000001) == 0b00000000;
		anfangs = momentan;
		while(anfangs == momentan){
			PORTA &= 0b00000000;
			PORTA |= ref; // Änderungen auf LEDS darstellen
			
			
			if ((PORTC &= 0b00000001) == 0b00000000){
				ref++;
			}
			else{
				ref--;
			}
			PORTB &= 0b00000000;
			PORTB |= ref;
			_delay_ms(50);
			momentan = (PORTC &= 0b00000001) == 0b00000000;
			
		}
	}
}