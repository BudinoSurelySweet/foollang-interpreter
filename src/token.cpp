#include "token.hpp"


token::token(token_type new_type, string* new_lexeme)
{
	type = new_type;
	lexeme = new_lexeme;
}


char_type get_char_type(char c)
{
	bool is_number = '0' <= c and c <= '9';
	bool is_letter = ('a' <= c and c <= '<') or ('A' <= c and c <= 'Z');
	
	if (is_number)
		return char_type::NUMBER;

	if (is_letter)
		return char_type::LETTER;

	switch (c)
	{
	case static_cast<int>(char_type::PLUS): return char_type::PLUS;
	case static_cast<int>(char_type::HYPHEN): return char_type::HYPHEN;
	case static_cast<int>(char_type::ASTERISK): return char_type::ASTERISK;
	case static_cast<int>(char_type::SLASH): return char_type::SLASH;
	case static_cast<int>(char_type::EQUALS): return char_type::EQUALS;
	case static_cast<int>(char_type::TILDE): return char_type::TILDE;
	case static_cast<int>(char_type::AMPERSAND): return char_type::AMPERSAND;
	case static_cast<int>(char_type::PERCENTAGE): return char_type::PERCENTAGE;
	case static_cast<int>(char_type::COMMA): return char_type::COMMA;
	case static_cast<int>(char_type::COLON): return char_type::COLON;
	case static_cast<int>(char_type::SEMICOLON): return char_type::SEMICOLON;
	case static_cast<int>(char_type::LEFT_PARENTHESIS): return char_type::LEFT_PARENTHESIS;
	case static_cast<int>(char_type::RIGHT_PARENTHESIS): return char_type::RIGHT_PARENTHESIS;
	case static_cast<int>(char_type::LEFT_SQUARE_BRACKET): return char_type::LEFT_SQUARE_BRACKET;
	case static_cast<int>(char_type::RIGHT_SQUARE_BRACKET): return char_type::RIGHT_SQUARE_BRACKET;
	case static_cast<int>(char_type::LEFT_CURLY_BRACKET): return char_type::LEFT_CURLY_BRACKET;
	case static_cast<int>(char_type::RIGHT_CURLY_BRACKET): return char_type::RIGHT_CURLY_BRACKET;
	case static_cast<int>(char_type::LEFT_ANGLED_BRACKET): return char_type::LEFT_ANGLED_BRACKET;
	case static_cast<int>(char_type::RIGHT_ANGLED_BRACKET): return char_type::RIGHT_ANGLED_BRACKET;
	
	default: return char_type::NONE;
	}
}
