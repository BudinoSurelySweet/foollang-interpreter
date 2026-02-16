#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

using namespace std;


enum class token_type
{
	NONE,
	I32,
	WORD,
	SEQUENCE_POINT,
	LEFT_ROUND_BRACKET,
	RIGHT_ROUND_BRACKET,
	OPERATOR_PLUS,
	OPERATOR_MINUS,
	OPERATOR_MULTIPLICATION,
	OPERATOR_DIVISION,
};


struct token
{
	token_type type;
	token* next;
	string value;
	
	token(token_type new_type, string new_value, token* new_next);
	
	~token();
};


token_type token_type_of(char c);


#endif
