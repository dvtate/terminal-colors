#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

#include <string>
#include <unordered_map>

#include "terminal_colors.hpp"


// used for unguarded numbers
//	ie - #333 = 333; #444444 = 444444; rgb(12,3,5) = 12 3 5
static inline unsigned short int countSpaces(char* str){
	if (str == NULL)
		return 0;

	unsigned short int ret = 0;
	while (*(str++))
		if (isspace(*str) || *str == ':' || *str == ',')
			ret++;

	// trailing spaces don't count
	str--;
	if (isspace(*str) || *str == ':' || *str == ',')
		ret--;

	return ret;

}

// used when differentiating between the color black
// and the color returned on error
static inline bool notBlack(char* clr){
	if (clr) {
		for (int16_t i = strlen(clr); i >= 0; i--)
			*(clr + i) = tolower(*(clr + i));

		return strcmp(clr, "black");
	}

	return true;
}

/*
static bool isHexColor(const char* color){
	if (!color)
		return color;

	do
		if (!isdigit(*color) || *color == 'a' || *color == 'b' || *color == 'c'
			|| *color == 'd' || *color == 'e' || *color == 'f')
			return false;
	while (*(color++));

	return true;

}
*/

RGB_t hexToClr(const char* hex){
	RGB_t ret = { { 0 } };

	for (uint8_t i = 0; i < 6; i++) {
		ret.val <<= 4;
		switch (hex[i]){
			case '0': break;
			case '1': ret.val += 1; break;
			case '2': ret.val += 2; break;
			case '3': ret.val += 3; break;
			case '4': ret.val += 4; break;
			case '5': ret.val += 5; break;
			case '6': ret.val += 6; break;
			case '7': ret.val += 7; break;
			case '8': ret.val += 8;	break;
			case '9': ret.val += 9;	break;
			case 'A': case 'a': ret.val += 10; break;
			case 'B': case 'b': ret.val += 11; break;
			case 'C': case 'c': ret.val += 12; break;
			case 'D': case 'd': ret.val += 13; break;
			case 'E': case 'e': ret.val += 14; break;
			case 'F': case 'f': ret.val += 15; break;
			default:
				return (RGB_t) { { 0 } };
		}
	}

	return ret;
}

RGB_t hex3ToClr(const char* hex){
	RGB_t ret = { { 0 } };

	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t b = 0; b < 2; b++){
			ret.val <<= 4;
			switch (hex[i]){
				case '0': break;
				case '1': ret.val += 1; break;
				case '2': ret.val += 2; break;
				case '3': ret.val += 3; break;
				case '4': ret.val += 4; break;
				case '5': ret.val += 5; break;
				case '6': ret.val += 6; break;
				case '7': ret.val += 7; break;
				case '8': ret.val += 8; break;
				case '9': ret.val += 9; break;
				case 'A': case 'a': ret.val += 10; break;
				case 'B': case 'b': ret.val += 11; break;
				case 'C': case 'c': ret.val += 12; break;
				case 'D': case 'd': ret.val += 13; break;
				case 'E': case 'e': ret.val += 14; break;
				case 'F': case 'f': ret.val += 15; break;
				default:
					return (RGB_t) { { 0 } };
			}
		}
	}

	return ret;
}


// ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// setFgColor()
// ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void fsetFgColor(FILE* file, const uint8_t red, const uint8_t green,
						const uint8_t blue)
	{ fprintf(file, "\x1B[38;2;%d;%d;%dm", red, green, blue); }

inline void fsetFgColor(FILE* file, const RGB_t color)
	{ fprintf(file, "\x1B[38;2;%d;%d;%dm", color.r, color.g, color.b); }

inline void setFgColor(const uint8_t red, const uint8_t green, const uint8_t blue)
	{ printf("\x1B[38;2;%d;%d;%dm", red, green, blue); }

inline void setFgColor(const RGB_t color)
	{ printf("\x1B[38;2;%d;%d;%dm", color.r, color.g, color.b); }

void fsetFgColor(FILE* file, const char* ccolor){

	// no color given, this could be desired (pass no error)
	if (!ccolor || strlen(ccolor) == 0)
		return;

	// copy the color out of the const qualifier
	char* color = (char*) malloc(strlen(ccolor));
	char* color_cpy = color;
	strcpy(color, ccolor);


	// skip leading spaces
	while (*color && isspace(*color))
		color++;

	// delete trailing spaces (is this needed?)
	while (isspace(color[strlen(color) - 1]))
		color[strlen(color) - 1] = '\0';


	// hex color
	if (*color == '#') {
		color++;
hex_color:
		if (strlen(color) == 3)
			fsetFgColor(file, hex3ToClr(color));
		else if (strlen(color) == 6)
			fsetFgColor(file, hexToClr(color));
		else
			fprintf(stderr, "ERROR: invalid hex color \"%s\"\n", color - 1);
		goto _end;
	}

	// rgb ()
	else if (strlen(color) >= 6
		&& (*color == 'r' || *color == 'R')
		&& (*(color + 1) == 'g' || *(color + 1) == 'G')
		&& (*(color + 2) == 'b' || *(color + 2) == 'B')
	){
		color += 3;

		// skip spaces
		while (*color && isspace(*color))
			color++;

		// skip to the first number
		if (*color && !isdigit(*color))
			color++;

tri_color:
		// tokenize the arguments into an array
		char* token = strtok(color, ", ");
		uint8_t vals[3];
		for (uint8_t i = 0; i < 3; i++) {

			if (token == NULL || *token == ')') {
				fprintf(stderr, "\aERROR: \"rgb(r,g,b)\" expected 3 arguments, "
						"%d provided\n", i + 1);
				break;
			}
			vals[i] = atoi(token);
			token = strtok(NULL, ", ");
		}

		fsetFgColor(file, vals[0], vals[1], vals[2]);
		goto _end;
	}

	// color by name (worst performance)
	else if (!isdigit(*color)) {
		RGB_t clr = nameToColor(color);

		// color was not in the list
		if (clr.val == 0 && notBlack(color))
			fprintf(stderr, "\aERROR: invalid HTML color. \"%s\" doesn\'t"
					"name a color\n", color);
		fsetFgColor(file, clr);

		goto _end;
	}
	else if (countSpaces(color) == 0)
		goto hex_color;
	else if (countSpaces(color) >= 2)
		goto tri_color;
	else
		fprintf(stderr, "\aERROR: invalid HTML color. \"%s\" doesn\'t"
				"name a color\n", color);
_end:
	free(color_cpy);

}


// ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// setBgColor()
// ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void setBgColor(const uint8_t red, const uint8_t green, const uint8_t blue)
	{ printf("\x1B[48;2;%d;%d;%dm", red, green, blue); }

inline void setBgColor(const RGB_t color)
	{ printf("\x1B[48;2;%d;%d;%dm", color.r, color.g, color.b); }

inline void fsetBgColor(FILE* file, const uint8_t red, const uint8_t green,
						const uint8_t blue)
	{ fprintf(file, "\x1B[48;2;%d;%d;%dm", red, green, blue); }

inline void fsetBgColor(FILE* file, const RGB_t color)
	{ fprintf(file, "\x1B[48;2;%d;%d;%dm", color.r, color.g, color.b); }

void fsetBgColor(FILE* file, const char* ccolor){

	// no color given, this could be desired (pass no error)
	if (!ccolor || strlen(ccolor) == 0)
		return;

	// copy the color out of the const qualifier
	char* color = (char*) malloc(strlen(ccolor));
	char* color_cpy = color;
	strcpy(color, ccolor);


	// skip leading spaces
	while (*color && isspace(*color))
		color++;

	// delete trailing spaces (is this needed?)
	while (isspace(color[strlen(color) - 1]))
		color[strlen(color) - 1] = '\0';


	// hex color
	if (*color == '#') {
		color++;

hex_color:
		if (strlen(color) == 3)
			fsetBgColor(file, hex3ToClr(color));
		else if (strlen(color) == 6)
			fsetBgColor(file, hexToClr(color));
		else
			fprintf(stderr,"\aERROR: invalid hex color \"%s\".\n", color - 1);
		goto _end;
	}

	// rgb ()
	else if (strlen(color) >= 6
		&& (*color == 'r' || *color == 'R')
		&& (*(color + 1) == 'g' || *(color + 1) == 'G')
		&& (*(color + 2) == 'b' || *(color + 2) == 'B')
	){
		color += 3;

		// skip spaces
		while (*color && isspace(*color))
			color++;

		// skip to the first number
		if (*color && !isdigit(*color))
			color++;

tri_color:
		// tokenize the arguments into an array
		char* token = strtok(color, ", ");
		uint8_t vals[3];
		for (uint8_t i = 0; i < 3; i++) {

			if (token == NULL || *token == ')') {
				fprintf(stderr, "\aERROR: \"rgb(r,g,b)\" expected 3 arguments, "
						"%d provided\n", i + 1);
				break;
			}
			vals[i] = atoi(token);
			token = strtok(NULL, ", ");
		}

		fsetBgColor(file, vals[0], vals[1], vals[2]);
		goto _end;
	}

	// color by name (worst performance)
	else if (!isdigit(*color)) {
		RGB_t clr = nameToColor(color);

		// color was not in the list
		if (clr.val == 0 && notBlack(color)) {
			fprintf(stderr, "\aERROR: invalid HTML color. \"%s\" doesn\'t"
					"name a color\n", color);
			return;
		}
		fsetBgColor(file, clr);

		goto _end;
	}
	// non-guarded colors
	else if (countSpaces(color) == 0)
		goto hex_color;
	else if (countSpaces(color) >= 2)
		goto tri_color;
	else
		fprintf(stderr, "\aERROR: invalid HTML color. \"%s\" doesn\'t"
				"name a color\n", color);


_end:
	free(color_cpy);

}



// prints an rgb format string
void color_printf(const uint8_t red, const uint8_t green, const uint8_t blue,
				  const char* format, ...)
{
	printf("\x1B[38;2;%d;%d;%dm", red, green, blue); // set color

	va_list args;
	va_start(args, format);
	vprintf(format, args); // print the format
	va_end(args);

	setFgColor(); // reset color

}

void color_fprintf(FILE* file, const uint8_t red, const uint8_t green,
				   const uint8_t blue, const char* format, ...)
{
	fprintf(file, "\x1B[38;2;%d;%d;%dm", red, green, blue); // set color

	va_list args;
	va_start(args, format);
	vfprintf(file, format, args); // print the format
	va_end(args);

	setFgColor(file); // reset color

}

// prints an rgb format string
static void color_fprintf(FILE* file, const uint8_t red, const uint8_t green,
						  const uint8_t blue, const char* format, va_list& args)
{
	fprintf(file, "\x1B[38;2;%d;%d;%dm", red, green, blue); // set color

	vfprintf(file, format, args); // print the format

	setFgColor(file); // reset color

}

void color_fprintf(FILE* file, const RGB_t color, const char* format, ...){
	fprintf(file, "\x1B[38;2;%d;%d;%dm", color.r, color.g, color.b); // set color

	va_list args;
	va_start(args, format);
	vfprintf(file, format, args); // print the format
	va_end(args);

	setFgColor(file); // reset color


}

void color_printf(const RGB_t color, const char* format, ...){
	printf("\x1B[38;2;%d;%d;%dm", color.r, color.g, color.b); // set color

	va_list args;
	va_start(args, format);
	vprintf(format, args); // print the format
	va_end(args);

	setFgColor(); // reset color


}

// prints an rgb format string
void color_fprintf(FILE* file, const RGB_t color, const char* format, va_list& args){
	fprintf(file, "\x1B[38;2;%d;%d;%dm", color.r, color.g, color.b); // set color

	vfprintf(file, format, args); // print the format
	setFgColor(file); // reset color

}


void color_fprintf(FILE* file, const char* ccolor, const char* format, ...){

	va_list args;
	va_start(args, format);

	// no color given, this could be desired (pass no error)
	if (!ccolor || strlen(ccolor) == 0) {
		vfprintf(file, format, args); // print the format
		return;
	}

	char* color = (char*) malloc(strlen(ccolor));
	char* color_cpy = color;
	strcpy(color, ccolor);

	// skip leading spaces
	while (*color && isspace(*color))
		color++;

	// delete trailing spaces
	while (isspace(color[strlen(color) - 1]))
		color[strlen(color) - 1] = '\0';


	// hex color
	if (*color == '#') {
		color++;
hex_color:
		if (strlen(color) == 3)
			color_fprintf(file, hex3ToClr(color), format, args);
		else if (strlen(color) == 6)
			color_fprintf(file, hexToClr(color), format, args);
		else {
			fprintf(stderr, "\aERROR: color_fprintf(): invalid hex color "
					"\"%s\"\n", color - 1);
			vfprintf(file, format, args); // print the format
		}

		va_end(args);
		goto end_printf;

	}

	// rgb ()
	else if (strlen(color) >= 6
		&& (*color == 'r' || *color == 'R')
		&& (*(color + 1) == 'g' || *(color + 1) == 'G')
		&& (*(color + 2) == 'b' || *(color + 2) == 'B')
	){
		color += 3;

		// skip spaces
		while (*color && isspace(*color))
			color++;

		// skip to the first number
		if (*color && !isdigit(*color))
			color++;

tri_color:
		char* token = strtok(color, ", ");
		uint8_t vals[3];
		for (uint8_t i = 0; i < 3; i++) {

			if (token == NULL || *token == ')') {
				fprintf(stderr, "\aERROR: color_fprintf(): rgb() expected 3 "
						"arguments, %d provided\n", i + 1);
				break;
			}
			vals[i] = atoi(token);
			token = strtok(NULL, ", ");
		}
		color_fprintf(file, vals[0], vals[1], vals[2], format, args);

		va_end(args);
		goto end_printf;
	}

	// color by name (worst performance)
	else if (!isdigit(*color)) {
		RGB_t clr = nameToColor(color);
		if (clr.val == 0 && notBlack(color)) {
			fprintf(stderr, "\aERROR: color_fprintf(): invalid HTML color. "
					"\"%s\" does\'t name a color\n", color);
			vfprintf(file, format, args);
			return;
		}
		color_fprintf(file, clr, format, args);
		va_end(args);


		goto end_printf;

	}
	// non-guarded colors
	else if (countSpaces(color) == 0)
		goto hex_color;
	else if (countSpaces(color) >= 2)
		goto tri_color;
	else
		fprintf(stderr, "\aERROR: color_fprintf(): invalid HTML color. "
				"\"%s\" does\'t name a color\n", color);


end_printf:
	free(color_cpy);

}

void color_printf(const char* ccolor, const char* format, ...){

	va_list args;
	va_start(args, format);

	// no color given, this could be desired (pass no error)
	if (!ccolor || strlen(ccolor) == 0) {
		vprintf(format, args); // print the format
		return;
	}

	char* color = (char*) malloc(strlen(ccolor));
	char* color_cpy = color;
	strcpy(color, ccolor);

	// skip leading spaces
	while (*color && isspace(*color))
		color++;

	// delete trailing spaces
	while (isspace(color[strlen(color) - 1]))
		color[strlen(color) - 1] = '\0';


	// hex color
	if (*color == '#') {
		color++;
hex_color:
		if (strlen(color) == 3)
			color_printf(hex3ToClr(color), format, args);
		else if (strlen(color) == 6)
			color_printf(hexToClr(color), format, args);
		else {
			fprintf(stderr, "\aERROR: color_printf(): invalid hex color "
				"\"%s\"\n", color - 1);
			vprintf(format, args); // print the format
		}
		va_end(args);

		goto end_printf;
	}

	// rgb ()
	else if (strlen(color) >= 6
		&& (*color == 'r' || *color == 'R')
		&& (*(color + 1) == 'g' || *(color + 1) == 'G')
		&& (*(color + 2) == 'b' || *(color + 2) == 'B')
	){
		color += 3;

		// skip spaces
		while (*color && isspace(*color))
			color++;

		// skip to the first number
		if (*color && !isdigit(*color))
			color++;

tri_color:
		char* token = strtok(color, ", ");
		uint8_t vals[3];
		for (uint8_t i = 0; i < 3; i++) {

			if (token == NULL || *token == ')') {
				fprintf(stderr, "\aERROR: color_printf(): rgb() expected 3 "
						"arguments, %d provided\n", i + 1);
				break;
			}
			vals[i] = atoi(token);
			token = strtok(NULL, ", ");
		}
		color_printf(vals[0], vals[1], vals[2], format, args);
		va_end(args);

		goto end_printf;
	}

	// color by name (worst performance)
	else if (!isdigit(*color)) {
		RGB_t clr = nameToColor(color);
		if (clr.val == 0 && notBlack(color)) {
			if (color)
			fprintf(stderr, "\aERROR: color_printf(): invalid HTML color. "
					"\"%s\" does\'t name a color\n", color);

			vprintf(format, args);
			return;
		}
		setFgColor(clr);
		vprintf(format, args);
		//color_printf(clr, format, args);


		va_end(args);
		goto end_printf;
	}
	// non-guarded colors (here there be dragons)
	else if (countSpaces(color) >= 2)
		goto tri_color;
	else if (countSpaces(color) == 0)
		// problems with unguarded hex colors:
		//		- if the color starts with an a,b,c,d,e,f, the hex will be confused
		// 		- should I even include this feature???
		goto hex_color;
	else
		fprintf(stderr, "\aERROR: color_printf(): invalid HTML color. "
				"\"%s\" does\'t name a color\n", color);


end_printf:
	free(color_cpy);

}


// A list of html colors
static std::unordered_map<std::string, RGB_t> colors_map = {
	// Common
	{ "black",		(RGB_t) { { 0 } }},
	{ "white",		(RGB_t) { { 16777215 } } },
	{ "red",		(RGB_t) { { 16711680 } } },
	{ "blue",		(RGB_t) { { 255 } } },
	{ "green",		(RGB_t) { { 32768 } } },
	{ "cyan",		(RGB_t) { { 65535 } } },
	{ "aqua",		(RGB_t) { { 65535 } } },
	{ "magenta",	(RGB_t) { { 16711935 } } },
	{ "fuchsia",	(RGB_t) { { 16711935 } } },
	{ "gray",		(RGB_t) { { 8421504 } } },
	{ "yellow",		(RGB_t) { { 16776960 } } },
	{ "pink",		(RGB_t) { { 16761035 } } },
	{ "orange",		(RGB_t) { { 16753920 } } },
	{ "gold",		(RGB_t) { { 16766720 } } },
	{ "purple",		(RGB_t) { { 8388736 } } },
	{ "brown",		(RGB_t) { { 10824234 } } },
	{ "navy",		(RGB_t) { { 128 } } },
	{ "indigo",		(RGB_t) { { 4915330 } } },

	// Reds
	{ "indianred",		(RGB_t) { { 13458524 } } },
	{ "lightcoral",		(RGB_t) { { 15761536 } } },
	{ "salmon",			(RGB_t) { { 16416882 } } },
	{ "darksalmon",		(RGB_t) { { 15308410 } } },
	{ "lightsalmon",	(RGB_t) { { 16752762 } } },
	{ "crimson",		(RGB_t) { { 14423100 } } },
	{ "darkred",		(RGB_t) { { 9109504 } } },
	{ "firebrick",		(RGB_t) { { 11674146 } } },

	// pinks
	{ "lightpink",		(RGB_t) { { 16758465 } } },
	{ "hotpink",		(RGB_t) { { 16738740 } } },
	{ "deeppink",		(RGB_t) { { 16716947 } } },
	{ "mediumvioletred",(RGB_t) { { 13047173 } } },
	{ "palevioletred",	(RGB_t) { { 14381203 } } },

	// oranges
	{ "lightsalmon",(RGB_t) { { 16752762 } } },
	{ "coral",		(RGB_t) { { 16744272 } } },
	{ "tomato",		(RGB_t) { { 16737095 } } },
	{ "orangered",	(RGB_t) { { 16729344 } } },
	{ "darkorange",	(RGB_t) { { 16747520 } } },

	// yellows
	{ "lightyellow",			(RGB_t) { { 16777184 } } },
	{ "lightgoldenrodyellow",	(RGB_t) { { 16775885 } } },
	{ "papayawhip",				(RGB_t) { { 16773077 } } },
	{ "moccasin",				(RGB_t) { { 16770229 } } },
	{ "peachpuff",				(RGB_t) { { 16767673 } } },
	{ "palegoldenrod",			(RGB_t) { { 15657130 } } },
	{ "khaki",					(RGB_t) { { 15787660 } } },
	{ "darkkhaki",				(RGB_t) { { 12433259 } } },

	// purples
	{ "lavender",		(RGB_t) { { 15132410 } } },
	{ "thistle",		(RGB_t) { { 14204888 } } },
	{ "plum",			(RGB_t) { { 14524637 } } },
	{ "violet",			(RGB_t) { { 15631086 } } },
	{ "orchid",			(RGB_t) { { 14315734 } } },
	{ "mediumorchid",	(RGB_t) { { 12211667 } } },
	{ "mediumpurple",	(RGB_t) { { 9662683 } } },
	{ "rebeccapurple",	(RGB_t) { { 6697881 } } },
	{ "blueviolet",		(RGB_t) { { 9055202 } } },
	{ "darkviolet",		(RGB_t) { { 9699539 } } },
	{ "darkorchid",		(RGB_t) { { 10040012 } } },
	{ "darkmagenta",	(RGB_t) { { 9109643 } } },
	{ "slateblue",		(RGB_t) { { 6970061 } } },
	{ "darkslateblue",	(RGB_t) { { 4734347 } } },
	{ "mediumslateblue",(RGB_t) { { 8087790 } } },

	// greens
	{ "greenyellow",		(RGB_t) { { 11403055 } } },
	{ "chartreuse",			(RGB_t) { { 8388352 } } },
	{ "lawngreen",			(RGB_t) { { 8190976 } } },
	{ "lime",				(RGB_t) { { 65280 } } },
	{ "limegreen",			(RGB_t) { { 3329330 } } },
	{ "palegreen",			(RGB_t) { { 10025880 } } },
	{ "lightgreen",			(RGB_t) { { 9498256 } } },
	{ "mediumspringgreen",	(RGB_t) { { 64154 } } },
	{ "springgreen",		(RGB_t) { { 64154 } } },
	{ "mediumseagreen",		(RGB_t) { { 65407 } } },
	{ "forestgreen",		(RGB_t) { { 2263842 } } },
	{ "darkgreen",			(RGB_t) { { 25600 } } },
	{ "yellowgreen",		(RGB_t) { { 10145074 } } },
	{ "olivedarb",			(RGB_t) { { 7048739 } } },
	{ "olive",				(RGB_t) { { 8421376 } } },
	{ "darkolivegreen",		(RGB_t) { { 5597999 } } },
	{ "mediumaquamarine",	(RGB_t) { { 6737322 } } },
	{ "darkseagreen",		(RGB_t) { { 9419915 } } },
	{ "lightseagreen",		(RGB_t) { { 2142890 } } },
	{ "darkcyan",			(RGB_t) { { 35723 } } },
	{ "teal",				(RGB_t) { { 32896 } } },

	// blues
	{ "lightcyan",		(RGB_t) { { 14745599 } } },
	{ "paleturquoise",	(RGB_t) { { 11529966 } } },
	{ "aquamarine",		(RGB_t) { { 8388564 } } },
	{ "turquoise",		(RGB_t) { { 4251856 } } },
	{ "mediumturquoise",(RGB_t) { { 4772300 } } },
	{ "darkturquoise",	(RGB_t) { { 52945 } } },
	{ "cadetblue",		(RGB_t) { { 6266528 } } },
	{ "steelblue",		(RGB_t) { { 4620980 } } },
	{ "lightsteelblue",	(RGB_t) { { 11584734 } } },
	{ "powderblue",		(RGB_t) { { 11591910 } } },
	{ "lightblue",		(RGB_t) { { 11393254 } } },
	{ "skyblue",		(RGB_t) { { 8900331 } } },
	{ "lightskyblue",	(RGB_t) { { 8900346 } } },
	{ "deepskyblue",	(RGB_t) { { 49151 } } },
	{ "dodgerblue",		(RGB_t) { { 2003199 } } },
	{ "cornflowerblue",	(RGB_t) { { 6591981 } } },
	{ "mediumslateblue",(RGB_t) { { 8087790 } } },
	{ "royalblue",		(RGB_t) { { 4286945 } } },
	{ "mediumblue",		(RGB_t) { { 205 } } },
	{ "darkblue",		(RGB_t) { { 139 } } },
	{ "midnightblue",	(RGB_t) { { 1644912 } } },

	// browns
	{ "cornsilk",		(RGB_t) { { 16775388 } } },
	{ "blanchedalmond",	(RGB_t) { { 16772045 } } },
	{ "bisque",			(RGB_t) { { 16770244 } } },
	{ "navajowhite",	(RGB_t) { { 16768685 } } },
	{ "wheat",			(RGB_t) { { 16113325 } } },
	{ "burlywood",		(RGB_t) { { 14596231 } } },
	{ "tan",			(RGB_t) { { 13808780 } } },
	{ "rosybrown",		(RGB_t) { { 12357519 } } },
	{ "sandybrown",		(RGB_t) { { 16032864 } } },
	{ "goldenrod",		(RGB_t) { { 14329120 } } },
	{ "peru",			(RGB_t) { { 13468991 } } },
	{ "chocolate",		(RGB_t) { { 13789470 } } },
	{ "saddlebrown",	(RGB_t) { { 9127187 } } },
	{ "sienna",			(RGB_t) { { 10506797 } } },
	{ "maroon",			(RGB_t) { { 8388608 } } },

	// whites
	{ "snow",			(RGB_t) { { 16775930 } } },
	{ "honeydew",		(RGB_t) { { 15794160 } } },
	{ "mintcream",		(RGB_t) { { 16121850 } } },
	{ "azure",			(RGB_t) { { 15794175 } } },
	{ "aliceblue",		(RGB_t) { { 15792383 } } },
	{ "ghostwhite",		(RGB_t) { { 16316671 } } },
	{ "whitesmoke",		(RGB_t) { { 16119285 } } },
	{ "seashell",		(RGB_t) { { 16774638 } } },
	{ "beige",			(RGB_t) { { 16119260 } } },
	{ "oldlace",		(RGB_t) { { 16643558 } } },
	{ "floralwhite",	(RGB_t) { { 16775920 } } },
	{ "ivory",			(RGB_t) { { 16777200 } } },
	{ "antiquewhite",	(RGB_t) { { 16444375 } } },
	{ "linen",			(RGB_t) { { 16445670 } } },
	{ "lavenderblush",	(RGB_t) { { 16773365 } } },
	{ "mistyrose",		(RGB_t) { { 16770273 } } },

	// greys
	{ "gainsboro",		(RGB_t) { { 14474460 } } },
	{ "lightgrey",		(RGB_t) { { 13882323 } } },
	{ "silver",			(RGB_t) { { 12632256 } } },
	{ "darkgrey",		(RGB_t) { { 11119017 } } },
	{ "dimgrey",		(RGB_t) { { 6908265 } } },
	{ "lightslategrey",	(RGB_t) { { 7833753 } } },
	{ "slategrey",		(RGB_t) { { 7372944 } } },
	{ "darkslategrey",	(RGB_t) { { 3100495 } } },
};

// Find color corresponding to given name
const RGB_t nameToColor(const char* const cname){
	if (!cname)
		return (RGB_t) { { 0 } };

	char name[strlen(cname) + 1];
	strcpy(name, cname);

	// set name to lowercase
	for (size_t i = 0; i <= strlen(name); i++)
		*(name + i) = tolower(*(name + i));

	return colors_map[name];
}
