#include "evaluator.hpp"

#include <charconv>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <variant>

#include "symbols.hpp"
#include "error_manager.hpp"
#include "token.hpp"
#include "number_manipulation.hpp"


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
	// // Unpack the left operand if it is a variable
	// if (get_value_access_type(operands->left->type) == value_access_type::INDIRECT_VALUE)
	// 	unpack_variable(operands->left);
	//
	// // Unpack the right operand if it is a variable
	// if (get_value_access_type(operands->right->type) == value_access_type::INDIRECT_VALUE)
	// 	unpack_variable(operands->right);
	//
	// op->lexeme = to_string(
	// 	atoi(operands->left->lexeme.c_str())
	// 	+ atoi(operands->right->lexeme.c_str())
	// );
	//
	// // HACK: Fare il tipo "dinamico", cioè che capisce di quale tipo deve diventare
	// op->type = token_type::I32;
}


static void subtraction_behavior(token* op, shared_ptr<operands_group> operands)
{
	// // Unpack the left operand if it is a variable
	// if (get_value_access_type(operands->left->type) == value_access_type::INDIRECT_VALUE)
	// 	unpack_variable(operands->left);
	//
	// // Unpack the right operand if it is a variable
	// if (get_value_access_type(operands->right->type) == value_access_type::INDIRECT_VALUE)
	// 	unpack_variable(operands->right);
	//
	// op->lexeme = to_string(
	// 	atoi(operands->left->lexeme.c_str())
	// 	- atoi(operands->right->lexeme.c_str())
	// );
	//
	// // HACK: Fare il tipo "dinamico", cioè che capisce di quale tipo deve diventare
	// op->type = token_type::I32;
}


static void multiplication_behavior(token* op, shared_ptr<operands_group> operands)
{
	// // Unpack the left operand if it is a variable
	// if (get_value_access_type(operands->left->type) == value_access_type::INDIRECT_VALUE)
	// 	unpack_variable(operands->left);
	//
	// // Unpack the right operand if it is a variable
	// if (get_value_access_type(operands->right->type) == value_access_type::INDIRECT_VALUE)
	// 	unpack_variable(operands->right);
	//
	// op->lexeme = to_string(
	// 	atoi(operands->left->lexeme.c_str())
	// 	* atoi(operands->right->lexeme.c_str())
	// );
	//
	// // HACK: Fare il tipo "dinamico", cioè che capisce di quale tipo deve diventare
	// op->type = token_type::I32;
}


static void division_behavior(token* op, shared_ptr<operands_group> operands)
{
	// // Unpack the left operand if it is a variable
	// if (get_value_access_type(operands->left->type) == value_access_type::INDIRECT_VALUE)
	// 	unpack_variable(operands->left);
	//
	// // Unpack the right operand if it is a variable
	// if (get_value_access_type(operands->right->type) == value_access_type::INDIRECT_VALUE)
	// 	unpack_variable(operands->right);
	//
	// op->lexeme = to_string(
	// 	atoi(operands->left->lexeme.c_str())
	// 	/ atoi(operands->right->lexeme.c_str())
	// );
	//
	// // HACK: Fare il tipo "dinamico", cioè che capisce di quale tipo deve diventare
	// op->type = token_type::I32;
}


static void var_declaration_behavior(token* op, shared_ptr<operands_group> operands)
{
	static auto operands_not_valid_err = new interpreter_error(error_id::OPERANDS_NOT_VALID);
	static auto non_existent_lang_type_err = new interpreter_error(error_id::NON_EXISTENT_LANG_TYPE);

	if (operands->right->type != token_type::WORD)
		exit_with(operands_not_valid_err->set_position(op));

	if (is_primitive_lang_type(operands->rightmost->type))
		exit_with(operands_not_valid_err->set_position(op));

	LangType value;

	switch (operands->rightmost->type) {
		case token_type::LANG_TYPE_C8:
			value = static_cast<char8_t>(0);
			break;

		case token_type::LANG_TYPE_C16:
			value = static_cast<char16_t>(0);
			break;

		case token_type::LANG_TYPE_C32:
			value = static_cast<char32_t>(0);
			break;

		case token_type::LANG_TYPE_I8:
			value = static_cast<int8_t>(0);
			break;

		case token_type::LANG_TYPE_I16:
			value = static_cast<int16_t>(0);
			break;

		case token_type::LANG_TYPE_I32:
			value = static_cast<int32_t>(0);
			break;

		case token_type::LANG_TYPE_I64:
			value = static_cast<int64_t>(0);
			break;

		case token_type::LANG_TYPE_U8:
			value = static_cast<uint8_t>(0);
			break;

		case token_type::LANG_TYPE_U16:
			value = static_cast<uint16_t>(0);
			break;

		case token_type::LANG_TYPE_U32:
			value = static_cast<uint32_t>(0);
			break;

		case token_type::LANG_TYPE_U64:
			value = static_cast<uint64_t>(0);
			break;

		case token_type::LANG_TYPE_F32:
			value = static_cast<float>(0);
			break;

		case token_type::LANG_TYPE_F64:
			value = static_cast<double>(0);
			break;

		default:
			exit_with(non_existent_lang_type_err->set_position(op));
			break;
	}

	global_symbols.define(operands->right->lexeme, value);

	op->type = operands->right->type;
	op->lexeme = operands->right->lexeme;
}


static void assignment_behavior(token* op, shared_ptr<operands_group> operands)
{
	// HACK: Check if the right_operand is a rvalue, cause if not I'll have to get its value from the has table
	// HACK: Find a way to check where the variable is stored

	LangType value;

	// BUG: Non viene mai scelto u32 o un tipo diverso da i32, perché in `create_token`
	// dico che i numeri sono sempre i32. Aggiungere il parser di numeri dentro
	// `create_token` così che posso fare la promozione a i64.

	if (operands->right->type == token_type::I32)
		printf("\n\nI32\n\n");
	else
		printf("\n\nnot I32\n\n");

	switch (operands->right->type) {
		case token_type::C8:
			// TODO: Add support for this
			break;
		case token_type::C16:
			// TODO: Add support for this
			break;
		case token_type::C32:
			// TODO: Add support for this
			break;
		case token_type::I8:
			value = parse_integer<int8_t>(operands->right->lexeme);
			break;
		case token_type::I16:
			value = parse_integer<int16_t>(operands->right->lexeme);
			break;
		case token_type::I32:
			value = parse_integer<int32_t>(operands->right->lexeme);
			break;
		case token_type::I64:
			value = parse_integer<int64_t>(operands->right->lexeme);
			break;
		case token_type::U8:
			value = parse_integer<uint8_t>(operands->right->lexeme);
			break;
		case token_type::U16:
			value = parse_integer<uint16_t>(operands->right->lexeme);
			break;
		case token_type::U32:
			value = parse_integer<uint32_t>(operands->right->lexeme);
			break;
		case token_type::U64:
			value = parse_integer<uint64_t>(operands->right->lexeme);
			break;
		case token_type::F32:
			value = stof(operands->right->lexeme);
			break;
		case token_type::F64:
			value = stod(operands->right->lexeme);
			break;
		default:
			// FIX: Push err: right operand is not a number
			break;
	}
	
	global_symbols.set(operands->left->lexeme, value);

	// Operator update
	op->type = operands->right->type;

	visit(
		[op](auto &value)
		{
			op->lexeme = to_string(value);
		},
		global_symbols.table().at(operands->left->lexeme)
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

	// TODO: Check if the operands are the intended types

	auto behavior = operators_behaviors.find(op->type);

	// Call the respective function based on the operator
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
