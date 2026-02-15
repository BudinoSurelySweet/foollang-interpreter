#include <iostream>

#include "lexer.hpp"
#include "linked_list.hpp"


token::token(token_type new_type, string new_value, token* new_next)
{
	type = new_type;
	value = new_value;
}


token::~token()
{
	delete next;
}


static token_type token_type_of(char c)
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


linked_list<token*>* lexer(string* source_code)
{
	string token_value = "";
	token_type prev_token_type = token_type::NONE;
	token_type curr_token_type = token_type::NONE;
	
	linked_list<token*>* token_list = new linked_list<token*>;
	
	for (string::iterator i = (*source_code).begin(); i != (*source_code).end(); ++i)
	{
		curr_token_type = token_type_of(*i);
		
		// TODO Migliorare la distinzione tra single token type e multiple token type
		bool is_single_token = (
			prev_token_type == token_type::LEFT_ROUND_BRACKET
			or prev_token_type == token_type::RIGHT_ROUND_BRACKET
			or prev_token_type == token_type::OPERATOR_PLUS
			or prev_token_type == token_type::OPERATOR_MINUS
			or prev_token_type == token_type::OPERATOR_MULTIPLICATION
			or prev_token_type == token_type::OPERATOR_DIVISION
		);
		
		if (
				(curr_token_type == token_type::NONE and token_value.length() > 0)
				or is_single_token
				or (prev_token_type != token_type::NONE and prev_token_type != curr_token_type)
		)
		{
			token* t = new token(prev_token_type, token_value, nullptr);
			
			token_list->append(t);
			
			token_value.clear();
		}
		
		if (curr_token_type != token_type::NONE)
		{
			token_value += *i;
		}
		
		prev_token_type = curr_token_type;
	}
	
	return token_list;
}
