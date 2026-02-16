#include "lexer.hpp"
#include "token.hpp"


llist<token*>* lexer(string* source_code)
{
	string token_value = "";
	token_type prev_token_type = token_type::NONE;
	token_type curr_token_type = token_type::NONE;
	
	llist<token*>* token_list = new llist<token*>;
	
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
