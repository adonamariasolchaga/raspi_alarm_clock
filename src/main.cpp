#include "lcd_display.hpp"

LCDdisplay myLCD(2,3,4,5,14,15,16,2); // DB4, DB5, DB6, DB7, RS, E, character_width, no_of_lines

int main() {
	myLCD.init();
	myLCD.clear() ;
	myLCD.cursor_off();
	myLCD.print_wrapped("AAAAAAAAAA");
	sleep_ms(2500);
	myLCD.clear();
	myLCD.goto_pos(0,0);
	sleep_ms(2500);
	myLCD.print("HELLO");
	sleep_ms(2500);	
	myLCD.clear();
	sleep_ms(2500);
	myLCD.cursor_off();
	sleep_ms(1000);
	myLCD.cursor_on(true);
	sleep_ms(1000);
	myLCD.display_off();
	return 0;
};
