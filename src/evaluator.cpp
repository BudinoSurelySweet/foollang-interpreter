#include "evaluator.hpp"


static pair<token*, token*> find_left_operands(vector<token>* list, size_t start_pos)
{
	pair<token*, token*> result = {nullptr, nullptr};

	for (int i = static_cast<int>(start_pos) - 1; i >= 0; i--)
	{
		token& operand_candidate = (*list)[i];

		if (operand_candidate.type != token_type::TOMBSTONE)
		{
			if (not result.first)
				result.first = &operand_candidate;
			else
			{
				result.second = &operand_candidate;

				break;
			}
		}
	}
	
	return result;
}


static pair<token*, token*> find_right_operands(vector<token>* list, size_t start_pos)
{
	pair<token*, token*> result = {nullptr, nullptr};
	
	for (size_t i = start_pos + 1; i < list->size(); i++)
	{
		token& operand = (*list)[i];

		if (operand.type != token_type::TOMBSTONE)
		{
			if (not result.first)
				result.first = &operand;
			else
			{
				result.second = &operand;

				break;
			}
		}
	}

	return result;
}


#pragma region Operations Behavior

static void addition_behavior(token* op, token* left_operand, token* right_operand)
{
	op->type = token_type::I32;
	
	// HACK Sistemare questa parte, non è ottimizzata
	if (left_operand->type == token_type::WORD)
	{
		visit(
			[op, right_operand](auto &value)
			{
				op->lexeme = to_string(value + atoi(right_operand->lexeme.c_str()));
			},
			variables_32bit.at(left_operand->lexeme)
		);
	}
	else
	{
		op->lexeme = to_string(
			atoi(left_operand->lexeme.c_str()) 
			+ atoi(right_operand->lexeme.c_str())
		);
	}
}

#pragma endregion


void evaluate_operands(vector<token>* tokens, token* op, size_t pos)
{
	static auto err = new interpreter_error(error_id::OPERANDS_NOT_VALID);

	auto [left_operand, leftmost_operand] = find_left_operands(tokens, pos);
	auto [right_operand, rightmost_operand] = find_right_operands(tokens, pos);

	operands_position intended_pos = get_operands_position(op->type);
	
	err->set_position(
		op->file_name,
		op->file_path,
		op->row,
		op->column
	);
	
	bool left_needed = intended_pos == operands_position::LEFT;
	bool left_doesnt_exists = not left_operand;

	bool right_needed = intended_pos == operands_position::RIGHT;
	bool right_doesnt_exists = not right_operand;

	bool left_and_right_needed = intended_pos == operands_position::LEFT_RIGHT;
	bool left_and_right_dont_exist = not left_operand or not right_operand;

	bool left_and_leftmost_needed = intended_pos == operands_position::LEFT_LEFT;
	bool left_and_leftmost_dont_exist = not left_operand or not leftmost_operand;
	
	bool right_and_rightmost_needed = intended_pos == operands_position::RIGHT_RIGHT;
	bool right_and_rightmost_dont_exist = not right_operand or not rightmost_operand;
	
	if (
		(left_needed and left_doesnt_exists)
		or (right_needed and right_doesnt_exists)
		or (left_and_right_needed and left_and_right_dont_exist)
		or (left_and_leftmost_needed and left_and_leftmost_dont_exist)
		or (right_and_rightmost_needed and right_and_rightmost_dont_exist)
	)
		exit_with(err);

	// TODO Create integer promotion
	// ...

	// ----------------------------------------------------------------------------------------------------

	// TODO Check if the operands are the intended types
	
	// TODO Spostare la logica degli operatori dentro una funzione designata per quel comportamento
	switch (op->type)
	{
	case token_type::PLUS:
		addition_behavior(op, left_operand, right_operand);

		break;
	
	case token_type::MINUS:
		op->type = token_type::I32;
		op->lexeme = to_string(
			atoi(left_operand->lexeme.c_str())
			- atoi(right_operand->lexeme.c_str())
		);
		
		break;

	case token_type::MULTIPLICATION:
		op->type = token_type::I32;
		op->lexeme = to_string(
			atoi(left_operand->lexeme.c_str())
			* atoi(right_operand->lexeme.c_str())
		);
		
		break;

	case token_type::DIVISION:
		op->type = token_type::I32;
		op->lexeme = to_string(
			atoi(left_operand->lexeme.c_str())
			/ atoi(right_operand->lexeme.c_str())
		);
		
		break;
	
	case token_type::VAR_DECLARATION:
		if (right_operand->type != token_type::WORD)
			exit_with(err);
		
		if (rightmost_operand->type != token_type::PRIMITIVE_TYPE)
			exit_with(err);
		
		// TODO Add other types
		if (rightmost_operand->lexeme == "i32")
			variables_32bit.insert({right_operand->lexeme, static_cast<int>(0)});

		op->type = right_operand->type;
		op->lexeme = right_operand->lexeme;

		break;
		
	case token_type::ASSIGNMENT:
		// HACK Check if the right_operand is a rvalue, cause if not I'll have to get its value from the has table
		// HACK Find a way to check where the variable is stored
		variables_32bit.at(left_operand->lexeme) = atoi(right_operand->lexeme.c_str());
		
		op->type = right_operand->type;
		
		visit(
			[op](auto &value123)
			{
				op->lexeme = to_string(value123);
			},
			variables_32bit.at(left_operand->lexeme)
		);

		break;
	
	default:
		break;
	}

	// Delete involved operands
	if (
		intended_pos == operands_position::LEFT
		or intended_pos == operands_position::LEFT_LEFT
		or intended_pos == operands_position::LEFT_RIGHT
	)
	{
		left_operand->lexeme.clear();
		left_operand->type = token_type::TOMBSTONE;
	}

	if (
		intended_pos == operands_position::RIGHT
		or intended_pos == operands_position::RIGHT_RIGHT
		or intended_pos == operands_position::LEFT_RIGHT
	)
	{
		right_operand->lexeme.clear();
		right_operand->type = token_type::TOMBSTONE;
	}
	
	if (intended_pos == operands_position::LEFT_LEFT)
	{
		leftmost_operand->lexeme.clear();
		leftmost_operand->type = token_type::TOMBSTONE;
	}

	if (intended_pos == operands_position::RIGHT_RIGHT)
	{
		rightmost_operand->lexeme.clear();
		rightmost_operand->type = token_type::TOMBSTONE;
	}
}


