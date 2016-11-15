#include "terminal_colors.hpp"


int main(){

	color_fprintf(stdout, "rgb(255,0,255)", "Hello world! %d\n", 666);
	color_fprintf(stderr, "#FF00ff", "Hello world!\n%s\n", "\tprintf in color! XDDDD");
	color_printf("#f0f", "Hello world!\n");
	color_printf("magenta", "Hello world%c \n", '!');
	color_printf(255, 0, 255, "Hello world!\n");

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
