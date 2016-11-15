#include "terminal_colors.hpp"


int main(){

	color_printf("rgb(255,0,255)", "hello world! %d\n", 666);
	color_printf("#FF00ff", "hello world!\n%s\n", "printf in color! ");
	color_printf("#f0f", "hello world!\n");
	color_printf("magenta", "hello world!\n");
	color_printf(255, 0, 255, "hello world!\n");

	RGB_t color;
	color.r = color.b = 255;
	color.g = 0;
	color_printf(color, "hello world!\n");

	setBgColor("cyan");
	setFgColor("rgb 255, 0, 0");
	printf("\nHello, I should be cyan...");
	setBgColor("#0f0");
	printf("derp...derp...derp\n");

	// set the background color back to default
	setBgColor();
	printf("derp...derp...derp\n");

	// reset colors, effects, etc.
	resetANSI();

	// I like blinking text
	// so far the onlt think I've seen this work in is xterm
	setTermEffect(TERM_EFF_BLINK);
	printf("resetANSI() resets all terminal colors and effects.\n");


	resetANSI(); // this should be at the end of all programs...
}
