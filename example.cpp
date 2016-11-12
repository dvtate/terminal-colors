#include "terminal_colors.hpp"


int main(){

	color_printf("rgb(255,0,255)", "hello world!\n");
	color_printf("#FF00ff", "hello world!\n");
	color_printf("#f0f", "hello world!\n");
	color_printf("magenta", "hello world!\n");
	color_printf(255, 0, 255, "hello world!\n");

	RGB_t color;
	color.r = color.b = 255;
	color.g = 0;
	color_printf(color, "hello world!\n");

}
