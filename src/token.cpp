#include "token.hpp"


token::token(token_type new_type, string new_value, token* new_next)
{
	type = new_type;
	value = new_value;
}


token::~token()
{
	delete next;
}


token_type token_type_of(char c)
{
	bool is_number = c >= '0' and c <= '9';
	bool is_word = (c >= 'A' and c <= 'Z') or (c >= 'a' and c <= 'z');
	bool is_sequence_point = c == ';';

	if (is_number) return token_type::I32;

	if (is_word) return token_type::WORD;
	
	if (is_sequence_point) return token_type::SEQUENCE_POINT;

	switch (c)
	{
		case '(': return token_type::LEFT_ROUND_BRACKET;
		case ')': return token_type::RIGHT_ROUND_BRACKET;
		case '+': return token_type::OPERATOR_PLUS;
		case '-': return token_type::OPERATOR_MINUS;
		case '*': return token_type::OPERATOR_MULTIPLICATION;
		case '/': return token_type::OPERATOR_DIVISION;
		default: return token_type::NONE;
	}
}
