#include <vector>
#include <memory>

#include "interpreter.hpp"
#include "token.hpp"
#include "linked_list.hpp"
#include "color.hpp"

// TODO Rimuovere
#include <iostream>

using namespace std;


// TODO Aggiungere la dichiarazione nell .hpp
static llist<lnode<token*>*>* create_operator_list(lnode<token*>* token_to_insert, bool end = false)
{
	int target_precedence_level;
	static bool is_list_initialized = false;
	static vector<vector<lnode<token*>*>> levels_of_precedence;
	
	if (not is_list_initialized)
	{
		is_list_initialized = true;
		levels_of_precedence.resize(14);
	}
		
	if (end)
	{
		auto* result_list = new llist<lnode<token*>*>;
		is_list_initialized = false;

		for (auto& row_vector : levels_of_precedence)
		{
			for (auto*& tok : row_vector)
			{
				result_list->append(tok);
			}
		}
		
		levels_of_precedence.clear();
		
		return result_list;
	}
	
	target_precedence_level = operator_precedence_to_int(token_to_insert->value->type);
	
	// Add the element to the corrispondent level
	//auto lvl = levels_of_precedence[target_level - 1];
	auto& lvl = levels_of_precedence.at(target_precedence_level - 1);
	lvl.push_back(token_to_insert);
	
	return nullptr;
}


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
	auto* expressions = new lmatrix<token*>;

	// It helps to keep track of the current expression inside the `expressions` matrix
	lnode<llist<token*>*>* curr_expr = nullptr;
	
	// A matrix that contain only the operators for each expression
	auto* operators = new lmatrix<lnode<token*>*>;

	// Keep track of the character position inside the `source_code`
	int character_pos = 0;

	// Initialize the first expression (linked list) and connect it to the bookmark (variable that keep track of the current expression)
	curr_expr = expressions->append(new llist<token*>);

	// Iterate over all the `source_code`
	for (auto target_char = source_code->begin(); target_char != source_code->end(); ++target_char, ++character_pos)
	{
		token* curr_token = create_token(source_code, character_pos);
		
		// Check if the token is available or not
		if (not curr_token)
			continue;
		
		lnode<token*>* curr_node = curr_expr->value->append(curr_token);
		
		if (is_operator(curr_token->type))
			create_operator_list(curr_node);

		if (curr_token->type == token_type::SEQUENCE_POINT)
		{
			// Create another expression and update the bookmark (variable that keep track of the current expression)
			curr_expr = expressions->append(new llist<token*>);
			operators->append(create_operator_list(nullptr, true));
		}
	}

	// WARNING Print expressions matrix
	for (auto curr_list = expressions->head; curr_list != nullptr; curr_list = curr_list->next)
	{
		cout << "- ";

		for (auto curr_node = curr_list->value->head; curr_node != nullptr; curr_node = curr_node->next)
		{
			token* t = curr_node->value;
			string s = (*source_code).substr(t->lexeme_pos, t->lexeme_len);
			cout << "[" << s << "] ";
		}
		
		cout << endl;
	}
	
	cout << endl;

	// WARNING Print operators matrix
	for (auto curr_list = operators->head; curr_list != nullptr; curr_list = curr_list->next)
	{
		cout << "- ";

		for (auto curr_node = curr_list->value->head; curr_node != nullptr; curr_node = curr_node->next)
		{
			token* t = curr_node->value->value;
			string s = (*source_code).substr(t->lexeme_pos, t->lexeme_len);
			cout << "[" << s << "] ";
		}
		
		cout << endl;
	}
	

	for (auto curr_list = operators->head; curr_list != nullptr; curr_list = curr_list->next)
	{
		for (auto curr_node = curr_list->value->head; curr_node != nullptr; curr_node = curr_node->next)
		{
			if (not curr_node->prev or not curr_node->next)
			{
				cout << color("[Error] Un operando è nullo.", RED) << endl;
				exit(1);
			}
			
			token* t = curr_node->value->value;
			auto* prev_node = curr_node->prev->value;
			auto* next_node = curr_node->next->value;
			
			if (prev_node->value->type == token_type::I32 and next_node->value->type == token_type::I32)
			{
				// TODO Continuare da qui

			}
		}
	}
}
