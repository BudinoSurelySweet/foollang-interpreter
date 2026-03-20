#include "color.hpp"


string color(const char* target_text, enum color text_color = RESET)
{
	return "\033[" + to_string(text_color) + "m" + target_text + "\033[0m";
}
