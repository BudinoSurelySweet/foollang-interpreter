#include <vector>
#include <memory>
#include <optional>

#include "interpreter.hpp"
#include "token.hpp"
#include "operator_manager.hpp"
#include "evaluator.hpp"
#include "color.hpp"

#include <iostream>

using namespace std;


static void update_position(char* character, size_t* row, size_t* column)
{
	if (*character == '\n')
	{
		*row += 1;
		*column = 0;
	}
	else
		*column += 1;
}


void interpret(string& source_code, string file_name, string file_path)
{
	operator_manager* operator_list_creator = new operator_manager;

	vector<token> source_code_tokenized;
	vector<size_t> operators_indices;
	vector<size_t> sequence_points_indices;
	
	size_t row = 1;
	size_t column = 0;
	size_t token_index = -1;
	size_t char_index = -1;
	
	bool comment_mode = false;

	// Iterate over all the `source_code`
	for (char character : source_code)
	{
		char_index++;

		update_position(&character, &row, &column);

		// End comment mode
		if (comment_mode and character == '\n')
			comment_mode = false;

		// Start comment mode
		if (character == '/' and source_code[char_index + 1] == '/')
			comment_mode = true;

		// Skip characters/tokens during comment mode
		if (comment_mode)
			continue;

		optional<token> curr_token = create_token(character, file_name, file_path, row, column);
		
		// Check if the token is available or not
		if (not curr_token)
			continue;
		
		source_code_tokenized.push_back(*curr_token);

		token_index++;
		
		if (is_operator(curr_token->type))
			operator_list_creator->add(curr_token->type, token_index);

		// WARNING: Quando mi ritrovo all'ultima riga se non c'è un sequence point non la esegue

		// Check if the token is a sequence point
		if (curr_token->type == token_type::SEQUENCE_POINT)
		{
			// Create another expression and update the bookmark (variable that keep track of the current expression)
			vector<size_t> operators = operator_list_creator->get();
			operator_list_creator->reset();
			operators_indices.insert(operators_indices.end(), operators.begin(), operators.end());
			
			sequence_points_indices.push_back(token_index);
		}
	}

	cout << endl << "Tokens:" << endl;
	
	for (auto &&token : source_code_tokenized)
	{
		cout << "[" << token.lexeme << "] ";
		
		if (token.type == token_type::SEQUENCE_POINT)
			cout << endl;
	}
	
	cout << endl << "Operators' precedence:" << endl;

	for (auto &&i : operators_indices)
	{
		cout << "[" << source_code_tokenized[i].lexeme << "] ";
	}
	
	// Evaluator
	for (auto &&operator_index : operators_indices)
	{
		evaluate_operands(&source_code_tokenized, &source_code_tokenized[operator_index], operator_index);
	}
	
	cout << endl << endl << "Results:" << endl;
	
	for (auto &&token : source_code_tokenized)
	{
		if (token.type == token_type::TOMBSTONE)
			continue;
		
		cout << "[" << token.lexeme << "] ";
		
		if (token.type == token_type::SEQUENCE_POINT)
			cout << endl;
	}

	delete operator_list_creator;
}
