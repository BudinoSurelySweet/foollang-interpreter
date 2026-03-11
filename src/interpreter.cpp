#include <vector>
#include <memory>
#include <optional>

#include "interpreter.hpp"
#include "token.hpp"
#include "operator_manager.hpp"
#include "color.hpp"

// TODO Rimuovere
#include <iostream>

using namespace std;


void interpret(string* source_code)
{
	vector<token> source_code_tokenized;
	vector<size_t> operators_indices;
	vector<size_t> sequence_points_indices;
	
	bool is_token_sequence_point = false;
	operator_manager* operator_list_creator = new operator_manager;

	// Iterate over all the `source_code`
	for (size_t token_index = -1; char character : *source_code)
	{
		optional<token> curr_token = create_token(character);
		
		// Check if the token is available or not
		if (not curr_token)
			continue;
		
		is_token_sequence_point = curr_token->type == token_type::SEQUENCE_POINT;
		token_index++;
		
		source_code_tokenized.push_back(*curr_token);
		
		if (is_operator(curr_token->type))
			operator_list_creator->add(curr_token->type, token_index);

		if (is_token_sequence_point )
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
	
	// TODO Print the results
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
