#include "interpreter.hpp"
#include "token.hpp"
#include "linked_list.hpp"
#include "color.hpp"

// TODO Rimuovere
#include <iostream>

using namespace std;


// TODO Aggiungere la dichiarazione nell .hpp
static token* create_token(string* source_code, size_t pos)
{
	static bool is_creating = false;
	static size_t context_pos = 0;
	static size_t context_size = 0;
	static token_type context_type = token_type::NONE;
	
	char character = (*source_code)[pos];
	char_type character_type = get_char_type(character);
	
	// Character is invalid
	if (get_token_type(character_type) == token_type::NONE and not is_creating)
		return nullptr;

	if (not is_creating)
	{
		is_creating = true;
		context_pos = pos;
		context_size = 1;
		context_type = get_token_type(character_type);
		
		return nullptr;
	}
	
	// Update context case and return token case
	if (can_char_be_in_token(context_type, character_type))
	{
		context_size++;
		
		return nullptr;
	}
	else
	{
		token* t = new token(context_type, context_pos, context_size);
		
		// Resetting static variables
		is_creating = false;
		context_pos = 0;
		context_size = 0;
		context_type = token_type::NONE;
		
		// Re-iter the same character because it was skipped to create the current token.
		// It's granted that it will start the token and return nullptr.
		create_token(source_code, pos);

		return t;
	}
}


void interpret(string* source_code)
{
	// A matrix that contain expressions (a linked list of tokens)
	lmatrix<token*>* expressions = new lmatrix<token*>;

	// It helps to keep track of the current expression
	lnode<llist<token*>*>* curr_expression = nullptr;
	
	int character_pos = 0;

	// Initialize the first expression (linked list) and connect it to the iterator
	curr_expression = expressions->append(new llist<token*>);

	// Iterate over all the `source_code`
	for (auto target_char = source_code->begin(); target_char != source_code->end(); ++target_char, ++character_pos)
	{
		token* curr_token = create_token(source_code, character_pos);
		
		// Check if the token is available or not
		if (not curr_token)
			continue;
		
		curr_expression->value->append(curr_token);
	
		if (curr_token->type == token_type::SEQUENCE_POINT)
		{
			expressions->append(new llist<token*>);
			curr_expression = curr_expression->next;
		}
	}

	// WARNING Print all matrix
	for (auto curr_list = expressions->head; curr_list != nullptr; curr_list = curr_list->next)
	{
		for (auto curr_node = curr_list->value->head; curr_node != nullptr; curr_node = curr_node->next)
		{
			token* t = curr_node->value;
			string s = (*source_code).substr(t->lexeme_pos, t->lexeme_len);
			cout << "[" << s << "] ";
		}
		
		cout << endl;
	}
}
