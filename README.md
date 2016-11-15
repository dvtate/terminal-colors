# terminal-colors
a colorful terminal text library

NOTE: this shouldn't be compatible with windows versions prior to windows 10, so I reccomend not using windows at all.
## example.cpp running in xterm <!-- update this plz-->
![example gif](https://raw.githubusercontent.com/dvtate/terminal-colors/master/example-2016-11-14_01.20.09.gif) 

# What's included?

## color_printf
  - `void color_printf(const uint8_t r, const uint8_t g, const uint8_t b, const char* format, ...)`: use the r, g, and b values to give a 24bit color. 
  - `void color_printf(const char* color, const char* format, ...)`: enter a valid html `color` (see this if you are confused http://www.w3schools.com/html/html_colors.asp ).
    + note that standard color names are generally slower than hex or rgb values

## setFgColor / setBgColor 
  - the functions are the same for setBgColor, however it acts on the background instead of the foreground
  - `void setFgColor(const uint8_t r, const uint8_t g, const uint8_t b)`: sets the foreground color to an rgb value
  - `void setFgColor(const char* color)`: sets the foreground color to a valid HTML color
  - `void setFgColor(void)`: sets the foreground color back to its original state

## setTermEffect
  - `setTermEffect(const uint8_t effect)`: sets the text effect to whichever text effect you would prefer, there are macros defined for eache effect, but I reccomend reading this ( https://en.wikipedia.org/wiki/ANSI_escape_code#CSI_codes ) wiki article. 
  
## resetANSI()
  - `resetANSI(void)`: resets the terminal back to its original/default state

## fsetFgColor, fsetBgColor, fsetTextEffect, fresetANSI(), etc.
  - these serve the same function as the regular functions, but allow the output to be piped to a file other than stdout.
  - the first argument is the file to replace stdout

