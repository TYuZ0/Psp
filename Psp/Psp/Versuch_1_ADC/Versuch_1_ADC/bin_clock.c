#include "bin_clock.h"

#include <avr/interrupt.h>
#include <avr/io.h>

//! Global variables
uint8_t hours;
uint8_t minutes; 
uint8_t seconds; 
uint16_t milliseconds;

/*!
 * \return The milliseconds counter of the current time.
 */
uint16_t getTimeMilliseconds() {
return milliseconds;
}

/*!
 * \return The seconds counter of the current time.
 */
uint8_t getTimeSeconds() {
return seconds;
}

/*!
 * \return The minutes counter of the current time.
 */
uint8_t getTimeMinutes() {
 return minutes;
}

/*!
 * \return The hour counter of the current time.
 */
uint8_t getTimeHours() {
return hours;
}

/*!
 *  Initializes the binary clock (ISR and global variables)
 */
void initClock(void) {
    // Set timer mode to CTC
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // Set prescaler to 1024
    TCCR0B |= (1 << CS02) | (1 << CS00);
    TCCR0B &= ~(1 << CS01);

    // Set compare register to 195 -> match every 10ms
    OCR0A = 195;

// Init variables
 hours = 0b00001100;
 minutes = 0b00111011;
 seconds = 0b00101101;
 milliseconds = 0b0000000000000000;

    // Enable timer and global interrupts
    TIMSK0 |= (1 << OCIE0A);
    sei();
}

/*!
 *  Updates the global variables to get a valid 12h-time
 */
void updateClock(void){
if(milliseconds == 0b0000001111101000){
	milliseconds &= 0b0000000000000000;
	seconds++;
}
if(seconds == 0b00001010 ){
	seconds &= 0b00000000;
	minutes++;
}
if(minutes == 0b00001010 ){
	minutes &= 0b00000000;
	hours++;
}
if(hours == 0b00001101){
	milliseconds &= 0b0000000000000000;
	seconds &= 0b00000000;
    minutes &= 0b00000000;
	hours &= 0b00000000;
	hours |= 0b00000001;
}
}

/*!
 *  ISR to increase millisecond-counter of the clock
 */
ISR(TIMER0_COMPA_vect) {
milliseconds += 10;
updateClock();
}
