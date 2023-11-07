#include "menu.h"

#include "adc.h"
#include "bin_clock.h"
#include "lcd.h"
#include "led.h"
#include "os_input.h"

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdlib.h>

/*!
 *  Hello world program.
 *  Shows the string 'Hello World!' on the display.
 */
void helloWorld(void) {
// Repeat until ESC gets pressed
while((os_getInput() & 0b00001000) != 0b00001000){
	 lcd_writeProgString(PSTR("Hallo Welt!"));
	 _delay_ms(500);
	 lcd_clear();
	 _delay_ms(500);
}
while((os_getInput() & 0b00001000 ) == 0b00001000){};
showMenu();
}

/*!
 *  Shows the clock on the display and a binary clock on the led bar.
 */
void displayClock(void) {
	//Intialisierung von Zeitvariablen
	uint8_t hours = getTimeHours();
	uint8_t minutes = getTimeMinutes();
	uint8_t seconds = getTimeSeconds();
	uint16_t milliseconds = getTimeMilliseconds();
	while((os_getInput() & 0b00001000) != 0b00001000){
		//LED-bar  auf Uhrzeit setzen
uint16_t clockVal = (hours << 12) | (minutes << 6 ) | (seconds << 0);
setLedBar(clockVal);
char timeString[12]; // 12 long String
snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d:%03d", getTimeHours(),getTimeMinutes(),getTimeSeconds(),milliseconds); // idk ob das so geht?
lcd_writeString(timeString);// prints number in HH:MM:SS:mmm fomrat

	}
}

/*!
 *  Shows the stored voltage values in the second line of the display.
 */
void displayVoltageBuffer(uint8_t displayIndex) {
// set courser to line 2 and erase this line before writing again
lcd_line2();
lcd_erase(2);
//get the storedVoltage that should be displaed
uint16_t storedVoltage = getStoredVoltage(displayIndex);
//display the Voltage in a 54/100 Voltag Format
char voltString[3];
snprintf(voltString,sizeof(voltString), "%03d",getBufferIndex()+1);
lcd_writeString(voltString);
lcd_writeProgString(PSTR("/"));
lcd_writeChar(getBufferSize());
lcd_writeVoltage(storedVoltage,1023,5);
}

/*!
 *  Shows the ADC value on the display and on the led bar.
 */
void displayAdc(void) {
	
	
	uint8_t bufferindex = 0;
while ((os_getInput() & 0b00001000) != 0b00001000){
	lcd_line1();
	lcd_erase(1);
	_delay_ms(100);
		uint16_t ledValue = 0;
		uint16_t adcResult = getAdcValue();
		lcd_writeVoltage(ledValue, 1023, 5);
		displayVoltageBuffer(bufferindex);
		while(adcResult >= 68){
			ledValue = (ledValue << 1);
			ledValue |= 0b00000010;
			adcResult -= 68;
		}
		setLedBar(adcResult);
	if((os_getInput() & 0b00000001) == 0b00000001){ // Enter? -> Wert speichern
		if((getBufferIndex() > 0) & (getBufferIndex() < (getBufferSize()-1)) ){
			storeVoltage();
		}
	}
    if((os_getInput() & 0b00000010) == 0b00000010){ // down? -> bufferindex--
		bufferindex --;
		if (bufferindex>0)
		{
			bufferindex++;
		}
		
	}
	if((os_getInput() & 0b00000100) == 0b00000100){ //up? -> bufferindex++
		bufferindex ++;
		if ((bufferindex < 100) & (bufferindex < getBufferIndex())) {
			bufferindex--;
		}
	}
	
	
}
}

/*! \brief Starts the passed program
 *
 * \param programIndex Index of the program to start.
 */
void start(uint8_t programIndex) {
    // Initialize and start the passed 'program'
    switch (programIndex) {
        case 0:
            lcd_clear();
            helloWorld();
            break;
        case 1:
            activateLedMask = 0xFFFF; // Use all LEDs
            initLedBar();
            initClock();
            displayClock();
            break;
        case 2:
            activateLedMask = 0xFFFE; // Don't use LED 0
            initLedBar();
            initAdc();
            displayAdc();
            break;
        default:
            break;
    }

    // Do not resume to the menu until all buttons are released
    os_waitForNoInput();
}

/*!
 *  Shows a user menu on the display which allows to start subprograms.
 */
void showMenu(void) {
    uint8_t pageIndex = 0;

    while (1) {
        lcd_clear();
        lcd_writeProgString(PSTR("Select:"));
        lcd_line2();

        switch (pageIndex) {
            case 0:
                lcd_writeProgString(PSTR("1: Hello world"));
                break;
            case 1:
                lcd_writeProgString(PSTR("2: Binary clock"));
                break;
            case 2:
                lcd_writeProgString(PSTR("3: Internal ADC"));
                break;
            default:
                lcd_writeProgString(PSTR("----------------"));
                break;
        }

        os_waitForInput();
        if (os_getInput() == 0b00000001) { // Enter
            os_waitForNoInput();
            start(pageIndex);
        } else if (os_getInput() == 0b00000100) { // Up
            os_waitForNoInput();
            pageIndex = (pageIndex + 1) % 3;
        } else if (os_getInput() == 0b00000010) { // Down
            os_waitForNoInput();
            if (pageIndex == 0) {
                pageIndex = 2;
            } else {
                pageIndex--;
            }
        }
    }
}
