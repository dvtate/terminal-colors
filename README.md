# terminal-colors [![Build Status](https://travis-ci.org/dvtate/terminal-colors.svg?branch=master)](https://travis-ci.org/dvtate/terminal-colors)
A colorful terminal text effects library. It makes using terminal effects ie- color changes, blinking text, rich text, etc. easier. This is actually just a small part of my scripting language, but I felt it could be useful on it's own.

NOTE: this shouldn't be compatible with windows versions prior to windows 10, so I reccomend not using windows at all.
## example.cpp running in xterm <!-- update this plz-->
![example gif](demo.gif) 

# What's included?

## color_printf()
  - `void color_printf(uint8_t r, uint8_t g, uint8_t b, const char* format, ...)`: use the r, g, and b values to give a 24bit color. 
  - `void color_printf(const char* color, const char* format, ...)`: enter a valid html `color` (see this if you are confused http://www.w3schools.com/html/html_colors.asp ).
    + note that standard color names are generally slower than hex or rgb values

## setFgColor() / setBgColor() 
  - the functions are the same for setBgColor, however it acts on the background instead of the foreground
  - `void setFgColor(uint8_t r, uint8_t g, uint8_t b)`: sets the foreground color to an rgb value
  - `void setFgColor(const char* color)`: sets the foreground color to a valid HTML color
  - `void setFgColor(void)`: sets the foreground color back to its original state

## setTermEffect()
  - `setTermEffect(uint8_t effect)`: sets the text effect to whichever text effect you would prefer, there are macros defined for eache effect, but I reccomend reading this ( https://en.wikipedia.org/wiki/ANSI_escape_code#CSI_codes ) wiki article. 
  
## resetANSI()
  - `resetANSI(void)`: resets the terminal back to its original/default state

## fsetFgColor(), fsetBgColor(), fsetTextEffect(), fresetANSI(), color_fprintf(), etc.
  - these serve the same function as the regular functions, but allow the output to be piped to a file other than stdout.
  - the first argument is the file to replace stdout

