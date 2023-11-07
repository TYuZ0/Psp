#include "led.h"

#include <avr/io.h>
// LEDs an = 0 im Outputregister
uint16_t activateLedMask = 0xFFFF;

/*!
 *  Initializes the led bar. Note: All PORTs will be set to output.
 */
void initLedBar(void) {
uint8_t	activateLedMask1 = (uint8_t)(activateLedMask & 0b0000000011111111);
uint8_t activateLedMask2 = (uint8_t)((activateLedMask >> 8) & 0b0000000011111111);

DDRA |= (0b11111111 & activateLedMask1);
DDRD |= (0b11111111 & activateLedMask2);
}

/*!
 *  Sets the passed value as states of the led bar (1 = on, 0 = off).
 */
void setLedBar(uint16_t value) {
uint16_t led = ~value;
uint8_t teil1 = (uint8_t)(led & 0b0000000011111111); // LED-Beleuchtung PORTA 
uint8_t teil2 = (uint8_t)((led >> 8) & 0b0000000011111111);//LED-Beleuchtung PORTD
uint8_t	activateLedMask1 = (uint8_t)(activateLedMask & 0b0000000011111111);
uint8_t activateLedMask2 = (uint8_t)((activateLedMask >> 8) & 0b0000000011111111);
PORTA &= 0b00000000;
PORTA |= (teil1 & activateLedMask1);
PORTD &= 0b00000000;
PORTD |= (teil2 & activateLedMask2);
}
