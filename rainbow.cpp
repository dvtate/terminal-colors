#include <stdio.h>
#include <inttypes.h>
#include <string.h>


#include "terminal_colors.hpp"

inline void cycle3(uint8_t& v0, uint8_t& v1, uint8_t& v2, uint8_t& curHi)
{
	// modify color
	if (curHi == 0)
		{ v0--; v1++; }
	else if (curHi == 1)
		{ v1--; v2++; }
	else if (curHi == 2)
		{ v2--; v0++; }

	// change colors as needed
	if (v0 <= 0 && curHi == 0)
		curHi = 1;
	else if (v1 <= 0 && curHi == 1)
		curHi = 2;
	else if (v2 <= 0 && curHi == 2)
		curHi = 0;
}

// prints a rainbow message
void rainbowPrint(const char* msg, const unsigned pace = 10)
{

	// our color
	RGB_t clr;
	clr.r = 255;
	clr.g = clr.b = 0;

	// color which starts high
	uint8_t curHi = 0;

	// which character are we on now?
	size_t chnum = 0;

	for (size_t cycle = 0; cycle < strlen(msg); cycle++) {
		
		// shift color	
		for (unsigned p = pace; p > 0; p--)
			cycle3(clr.r, clr.g, clr.b, curHi);

		// print color
		color_printf(clr, "%c", msg[chnum++]);
	}




	setFgColor(); // reset fg color


}

int main()
{

	rainbowPrint("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do\
 eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,\
 quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis\
 aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla\
 pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia\
 deserunt mollit anim id est laborum.\n", 27);
}
