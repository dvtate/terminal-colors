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

	setBgColor("#0ff");
	setFgColor("#800");
	printf("\nHello, I should be cyan...");
	setBgColor("#0f0");
	printf("derp...derp...derp\n");

	resetASCII();
	setEffect(TERM_CLR_BLINK);
	printf("resetASCII() resets all terminal colors and effects.\n");


	resetASCII(); // this should be at the end of all programs...
}
