#include <iostream>

#include "token.hpp"
#include "color.hpp"

using namespace std;


token::token(token_type new_type, size_t new_lexeme_pos, size_t new_lexeme_len)
{
	type = new_type;
	lexeme_pos = new_lexeme_pos;
	lexeme_len = new_lexeme_len;
}


// HACK Provare ad automatizzare questo processo
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
	case (int) char_type::PLUS: return char_type::PLUS;
	case (int) char_type::HYPHEN: return char_type::HYPHEN;
	case (int) char_type::ASTERISK: return char_type::ASTERISK;
	case (int) char_type::SLASH: return char_type::SLASH;
	case (int) char_type::EQUALS: return char_type::EQUALS;
	case (int) char_type::TILDE: return char_type::TILDE;
	case (int) char_type::AMPERSAND: return char_type::AMPERSAND;
	case (int) char_type::PERCENTAGE: return char_type::PERCENTAGE;
	case (int) char_type::COMMA: return char_type::COMMA;
	case (int) char_type::COLON: return char_type::COLON;
	case (int) char_type::SEMICOLON: return char_type::SEMICOLON;
	case (int) char_type::LEFT_PARENTHESIS: return char_type::LEFT_PARENTHESIS;
	case (int) char_type::RIGHT_PARENTHESIS: return char_type::RIGHT_PARENTHESIS;
	case (int) char_type::LEFT_SQUARE_BRACKET: return char_type::LEFT_SQUARE_BRACKET;
	case (int) char_type::RIGHT_SQUARE_BRACKET: return char_type::RIGHT_SQUARE_BRACKET;
	case (int) char_type::LEFT_CURLY_BRACKET: return char_type::LEFT_CURLY_BRACKET;
	case (int) char_type::RIGHT_CURLY_BRACKET: return char_type::RIGHT_CURLY_BRACKET;
	case (int) char_type::LEFT_ANGLED_BRACKET: return char_type::LEFT_ANGLED_BRACKET;
	case (int) char_type::RIGHT_ANGLED_BRACKET: return char_type::RIGHT_ANGLED_BRACKET;
	
	default: return char_type::NONE;
	}
}


// HACK Provare ad automatizzare questo processo
token_type get_token_type(char_type c)
{
	switch (c)
	{
	case char_type::NUMBER: return token_type::I32;
	case char_type::LETTER: return token_type::WORD;
	case char_type::PLUS: return token_type::PLUS;
	case char_type::HYPHEN: return token_type::MINUS;
	case char_type::ASTERISK: return token_type::MULTIPLICATION;
	case char_type::SLASH: return token_type::DIVISION;
	case char_type::EQUALS: return token_type::ASSIGNMENT;
	case char_type::PERCENTAGE: return token_type::REMAINDER;
	case char_type::COMMA: return token_type::COMMA;
	case char_type::SEMICOLON: return token_type::SEQUENCE_POINT;
	
	default:
		return token_type::NONE;
		break;
	}
}


bool is_operator(token_type t)
{
	if (t > token_type::OPERATOR_H and t < token_type::END_OPERATOR_H)
		return true;

	return false;
}


bool can_char_be_in_token(token_type t, char_type c)
{
	if (c == char_type::NUMBER)
		switch (t)
		{
		case token_type::I8:
		case token_type::I16:
		case token_type::I32:
		case token_type::I64:
			return true;
		
		default:
			return false;
		}

	if (t == token_type::WORD)
		switch (c)
		{
		case char_type::LETTER:
		case char_type::NUMBER:
			return true;
		
		default:
			return false;
		}
	
	// Insert other cases here

	return false;
}
