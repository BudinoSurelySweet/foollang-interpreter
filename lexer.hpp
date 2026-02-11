#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>

#include "linked_list.hpp"

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
	string value;
	token* next;
	
	token(token_type new_type, string new_value, token* new_next);
	
	~token();
};


linked_list<token*>* lexer(string* source_code);


#endif
