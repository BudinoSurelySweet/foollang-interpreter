#include "evaluator.hpp"
#include "token.hpp"
#include <memory>


operands_group::operands_group(token* leftmost, token* left, token* right, token* rightmost)
{
	this->leftmost = leftmost;
	this->left = left;
	this->right = right;
	this->rightmost = rightmost;
}


#pragma region Finders

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

#pragma endregion


#pragma region Operations Behavior

static void addition_behavior(token* op, shared_ptr<operands_group> operands)
{
	op->type = token_type::I32;
	
	// HACK: Sistemare questa parte, non è ottimizzata
	if (operands->left->type == token_type::WORD)
	{
		visit(
			[op, operands](auto &value)
			{
				op->lexeme = to_string(value + atoi(operands->right->lexeme.c_str()));
			},
			variables_32bit.at(operands->left->lexeme)
		);
	}
	else
	{
		op->lexeme = to_string(
			atoi(operands->left->lexeme.c_str()) 
			+ atoi(operands->right->lexeme.c_str())
		);
	}
}


static void subtraction_behavior(token* op, shared_ptr<operands_group> operands)
{
	op->type = token_type::I32;
	op->lexeme = to_string(
		atoi(operands->left->lexeme.c_str())
		- atoi(operands->right->lexeme.c_str())
	);
}


static void multiplication_behavior(token* op, shared_ptr<operands_group> operands)
{
	op->type = token_type::I32;
	op->lexeme = to_string(
		atoi(operands->left->lexeme.c_str())
		* atoi(operands->right->lexeme.c_str())
	);
}


static void division_behavior(token* op, shared_ptr<operands_group> operands)
{
	op->type = token_type::I32;
	op->lexeme = to_string(
		atoi(operands->left->lexeme.c_str())
		/ atoi(operands->right->lexeme.c_str())
	);
}


static void var_declaration_behavior(token* op, shared_ptr<operands_group> operands)
{
	static auto err = new interpreter_error(error_id::OPERANDS_NOT_VALID);

	err->set_position(
		op->file_name,
		op->file_path,
		op->row,
		op->column
	);

	if (operands->right->type != token_type::WORD)
		exit_with(err);
	
	if (operands->rightmost->type != token_type::PRIMITIVE_TYPE)
		exit_with(err);
	
	// TODO: Add other types
	if (operands->rightmost->lexeme == "i32")
		variables_32bit.insert({operands->right->lexeme, static_cast<int>(0)});

	op->type = operands->right->type;
	op->lexeme = operands->right->lexeme;
}


static void assignment_behavior(token* op, shared_ptr<operands_group> operands)
{
	// HACK: Check if the right_operand is a rvalue, cause if not I'll have to get its value from the has table
	// HACK: Find a way to check where the variable is stored
	variables_32bit.at(operands->left->lexeme) = atoi(operands->right->lexeme.c_str());
	
	op->type = operands->right->type;
	
	visit(
		[op](auto &value123)
		{
			op->lexeme = to_string(value123);
		},
		variables_32bit.at(operands->left->lexeme)
	);
}


#pragma endregion


void evaluate_operands(vector<token>* tokens, token* op, size_t pos)
{
	static const unordered_map<token_type, function<void(token*, shared_ptr<operands_group>)>> operators_behaviors = {
		{ token_type::PLUS, addition_behavior },
		{ token_type::MINUS, subtraction_behavior },
		{ token_type::MULTIPLICATION, multiplication_behavior },
		{ token_type::DIVISION, division_behavior },
		{ token_type::VAR_DECLARATION, var_declaration_behavior },
		{ token_type::ASSIGNMENT, assignment_behavior },
	};

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

	shared_ptr<operands_group> operands = make_shared<operands_group>(leftmost_operand, left_operand, right_operand, rightmost_operand);

	// TODO: Create integer promotion

	// TODO: Check if the operands are the intended types

	auto behavior = operators_behaviors.find(op->type);
	
	if (behavior != operators_behaviors.end())
		behavior->second(op, operands);

	// Kill involved operands
	if (
		intended_pos == operands_position::LEFT
		or intended_pos == operands_position::LEFT_LEFT
		or intended_pos == operands_position::LEFT_RIGHT
	)
		left_operand->become_tombstone();

	if (
		intended_pos == operands_position::RIGHT
		or intended_pos == operands_position::RIGHT_RIGHT
		or intended_pos == operands_position::LEFT_RIGHT
	)
		right_operand->become_tombstone();
	
	if (intended_pos == operands_position::LEFT_LEFT)
		leftmost_operand->become_tombstone();

	if (intended_pos == operands_position::RIGHT_RIGHT)
		rightmost_operand->become_tombstone();
}
