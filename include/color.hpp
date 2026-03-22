#pragma once

#include <string>

using namespace std;


enum color
{
	RESET = 0,

	BLACK = 30,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	
	BRIGHT_BLACK = 90,
	BRIGHT_RED,
	BRIGHT_GREEN,
	BRIGHT_YELLOW,
	BRIGHT_BLUE,
	BRIGHT_MAGENTA,
	BRIGHT_CYAN,
	BRIGHT_WHITE,
};


//string color(char* target_text, enum color text_color, enum color style, enum color bg_color);


string color(const char* target_text, enum color text_color);

