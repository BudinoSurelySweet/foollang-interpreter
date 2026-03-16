#include <unordered_map>
#include <iostream>

#include "token.hpp"
#include "error_manager.hpp"

using namespace std;


token::token(token_type new_type, string new_lexeme, string new_file_name, string new_file_path, size_t new_row, size_t new_column)
{
	type = new_type;
	lexeme = new_lexeme;

	file_name = new_file_name;
	file_path = new_file_path;
	row = new_row;
	column = new_column;
}


optional<token> create_token(char character, string target_file_name, string target_file_path, size_t target_row, size_t target_column)
{
	static bool is_creating = false;
	static token_type context_type = token_type::NONE;
	static string context = "";

	static string file_name;
	static string file_path;
	static size_t row;
	static size_t column;

	char_type character_type = get_char_type(character);
	
	// Character is invalid
	if (get_token_type(character_type) == token_type::NONE and not is_creating)
		return nullopt;

	if (not is_creating)
	{
		is_creating = true;
		context.push_back(character);
		context_type = get_token_type(character_type);

		file_name = target_file_name;
		file_path = target_file_path;
		row = target_row;
		column = target_column;
		
		return nullopt;
	}

	// Update context case and return token case
	if (can_char_be_in_token(context_type, character_type))
	{
		context.push_back(character);
		
		return nullopt;
	}
	else
	{
		token t = token(context_type, context, file_name, file_path, row, column);
		
		// Resetting static variables
		is_creating = false;
		context_type = token_type::NONE;

		context.clear();
		
		// Re-iter the same character because it was skipped to create the current token.
		// It's granted that it will start the token and return nullptr.
		create_token(character, target_file_name, target_file_path, target_row, target_column);

		return t;
	}
}


char_type get_char_type(char c)
{
	bool is_number = '0' <= c and c <= '9';
	bool is_letter = ('a' <= c and c <= 'z') or ('A' <= c and c <= 'Z');
	
	if (is_number)
		return char_type::NUMBER;

	if (is_letter)
		return char_type::LETTER;

	static const unordered_map<int, char_type> char_map = {
		{ (int) char_type::PLUS, char_type::PLUS },
		{ (int) char_type::HYPHEN, char_type::HYPHEN },
		{ (int) char_type::ASTERISK, char_type::ASTERISK },
		{ (int) char_type::SLASH, char_type::SLASH },
		{ (int) char_type::EQUALS, char_type::EQUALS },
		{ (int) char_type::TILDE, char_type::TILDE },
		{ (int) char_type::AMPERSAND, char_type::AMPERSAND },
		{ (int) char_type::PERCENTAGE, char_type::PERCENTAGE },
		{ (int) char_type::COMMA, char_type::COMMA },
		{ (int) char_type::COLON, char_type::COLON },
		{ (int) char_type::SEMICOLON, char_type::SEMICOLON },
		{ (int) char_type::LEFT_PARENTHESIS, char_type::LEFT_PARENTHESIS },
		{ (int) char_type::RIGHT_PARENTHESIS, char_type::RIGHT_PARENTHESIS },
		{ (int) char_type::LEFT_SQUARE_BRACKET, char_type::LEFT_SQUARE_BRACKET },
		{ (int) char_type::RIGHT_SQUARE_BRACKET, char_type::RIGHT_SQUARE_BRACKET },
		{ (int) char_type::LEFT_CURLY_BRACKET, char_type::LEFT_CURLY_BRACKET },
		{ (int) char_type::RIGHT_CURLY_BRACKET, char_type::RIGHT_CURLY_BRACKET },
		{ (int) char_type::LEFT_ANGLED_BRACKET, char_type::LEFT_ANGLED_BRACKET },
		{ (int) char_type::RIGHT_ANGLED_BRACKET, char_type::RIGHT_ANGLED_BRACKET },
	};
	
	auto tuple = char_map.find(c);
	
	if (tuple != char_map.end())
		return tuple->second;
	
	return char_type::NONE;
}


token_type get_token_type(char_type c)
{
	static const unordered_map<char_type, token_type> token_map = {
		{ char_type::NUMBER, token_type::I32 },
		{ char_type::LETTER, token_type::WORD },
		{ char_type::PLUS, token_type::PLUS },
		{ char_type::HYPHEN, token_type::MINUS },
		{ char_type::ASTERISK, token_type::MULTIPLICATION },
		{ char_type::SLASH, token_type::DIVISION },
		{ char_type::EQUALS, token_type::ASSIGNMENT },
		{ char_type::PERCENTAGE, token_type::REMAINDER },
		{ char_type::COMMA, token_type::COMMA },
		{ char_type::SEMICOLON, token_type::SEQUENCE_POINT },
	};

	auto tuple = token_map.find(c);
	
	if (tuple != token_map.end())
		return tuple->second;
	
	return token_type::NONE;
}


int get_operator_precedence(token_type t)
{
	static const unordered_map<token_type, int> operator_precedence = {
		{ token_type::MULTIPLICATION, 3 },
		{ token_type::DIVISION, 3 },
		{ token_type::REMAINDER, 3 },
		{ token_type::PLUS, 4 },
		{ token_type::MINUS, 4 },
		{ token_type::ASSIGNMENT, 13 },
		{ token_type::COMMA, 14 },
	};

	auto tuple = operator_precedence.find(t);
	
	if (tuple != operator_precedence.end())
		return tuple->second;
	
	return -1;
}


bool are_operands_valid(token* target_operator, token_type first_operand, token_type second_operand)
{
	operator_arity arity = get_operator_arity(target_operator->type);
	
	if (arity == operator_arity::NONE)
		return false;

	bool first_operand_is_number = (
		first_operand == token_type::I8 or first_operand == token_type::I16
		or first_operand == token_type::I32 or first_operand == token_type::I64
	);
	
	bool second_operand_is_number = (
		second_operand == token_type::I8 or second_operand == token_type::I16
		or second_operand == token_type::I32 or second_operand == token_type::I64
	);

	switch (target_operator->type)
	{
	case token_type::MULTIPLICATION:
	case token_type::DIVISION:
	case token_type::PLUS:
	case token_type::MINUS:
		if (first_operand_is_number and second_operand_is_number)
			return true;
		
		return false;
	
	default:
		auto err = new interpreter_error(error_id::NON_EXISTENT_OPERATOR);

		err->file_name = target_operator->file_name;
		err->file_path = target_operator->file_path;
		err->row = target_operator->row;
		err->column = target_operator->column;

		exit_with(err);
	}

	return false;
}


operator_arity get_operator_arity(token_type t)
{
	if (not is_operator(t))
		return operator_arity::NONE;
	
	switch (t)
	{
	case token_type::MULTIPLICATION:
	case token_type::DIVISION:
	case token_type::REMAINDER:
	case token_type::PLUS:
	case token_type::MINUS:
	case token_type::COMMA:
		return operator_arity::BINARY;
	
	default:
		break;
	}
	
	return operator_arity::NONE;
}


void evaluate_operands(vector<token>* tokens, token* op, size_t pos)
{
	// TODO Create integer promotion
	// ...
	
	token* first_operand = nullptr;
	token* second_operand = nullptr;

	// Search the first operand
	for (int i = static_cast<int>(pos) - 1; i >= 0; i--)
	{
		token& operand = (*tokens)[i];

		if (operand.type != token_type::TOMBSTONE)
		{
			first_operand = &operand;

			break;
		}
	}
	
	if (not first_operand)
	{
		auto err = new interpreter_error(
			error_id::OPERANDS_NOT_VALID,
			"The left (first) operand of '" + op->lexeme + "' is not valid."
		);
		
		err->file_name = op->file_name;
		err->file_path = op->file_path;
		err->row = op->row;
		err->column = op->column;

		exit_with(err);
	}

	// Search the second operand
	for (size_t i = pos + 1; i < tokens->size(); i++)
	{
		token& operand = (*tokens)[i];

		if (operand.type != token_type::TOMBSTONE)
		{
			second_operand = &operand;

			break;
		}
	}

	if (not second_operand)
	{
		auto err = new interpreter_error(
			error_id::OPERANDS_NOT_VALID,
			"The right (second) operand of '" + op->lexeme + "' is not valid."
		);
		
		err->file_name = op->file_name;
		err->file_path = op->file_path;
		err->row = op->row;
		err->column = op->column;

		exit_with(err);
	}
	
	if (not are_operands_valid(op, first_operand->type, second_operand->type))
	{
		auto err = new interpreter_error(error_id::OPERANDS_NOT_VALID);

		err->file_name = op->file_name;
		err->file_path = op->file_path;
		err->row = op->row;
		err->column = op->column;

		exit_with(err);
	}
	
	auto fop = atoi(first_operand->lexeme.c_str());
	auto sop = atoi(second_operand->lexeme.c_str());

	switch (op->type)
	{
	case token_type::PLUS:
		op->lexeme = to_string(fop + sop);
		op->type = token_type::I32;

		break;
	
	case token_type::MINUS:
		op->lexeme = to_string(fop - sop);
		op->type = token_type::I32;
		
		break;

	case token_type::MULTIPLICATION:
		op->lexeme = to_string(fop * sop);
		op->type = token_type::I32;
		
		break;

	case token_type::DIVISION:
		op->lexeme = to_string(fop / sop);
		op->type = token_type::I32;
		
		break;
	
	default:
		break;
	}

	first_operand->lexeme.clear();
	first_operand->type = token_type::TOMBSTONE;

	second_operand->lexeme.clear();
	second_operand->type = token_type::TOMBSTONE;
}


bool is_operator(token_type t)
{
	if (t > token_type::OPERATOR_H and t < token_type::END_OPERATOR_H)
		return true;

	return false;
}


bool can_char_be_in_token(token_type t, char_type c)
{
	if (c == char_type::NUMBER)
		switch (t)
		{
		case token_type::I8:
		case token_type::I16:
		case token_type::I32:
		case token_type::I64:
			return true;
		
		default:
			return false;
		}

	if (t == token_type::WORD)
		switch (c)
		{
		case char_type::LETTER:
		case char_type::NUMBER:
			return true;
		
		default:
			return false;
		}
	
	// Insert other cases here

	return false;
}
