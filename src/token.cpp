#include <unordered_map>
#include <iostream>

#include "token.hpp"
#include "error_manager.hpp"

using namespace std;


token::token(token_type new_type, string new_lexeme)
{
	type = new_type;
	lexeme = new_lexeme;
}


optional<token> create_token(char character)
{
	static bool is_creating = false;
	static token_type context_type = token_type::NONE;
	static string context = "";

	char_type character_type = get_char_type(character);
	
	// Character is invalid
	if (get_token_type(character_type) == token_type::NONE and not is_creating)
		return nullopt;

	if (not is_creating)
	{
		is_creating = true;
		context.push_back(character);
		context_type = get_token_type(character_type);
		
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
		token t = token(context_type, context);
		
		// Resetting static variables
		is_creating = false;
		context_type = token_type::NONE;

		context.clear();
		
		// Re-iter the same character because it was skipped to create the current token.
		// It's granted that it will start the token and return nullptr.
		create_token(character);

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


bool are_operands_valid(token_type target_operator, token_type first_operand, token_type second_operand)
{
	operator_arity arity = get_operator_arity(target_operator);
	
	if (arity == operator_arity::NONE or arity != operator_arity::BINARY)
		return false;

	bool first_operand_is_number = (
		first_operand == token_type::I8 or first_operand == token_type::I16
		or first_operand == token_type::I32 or first_operand == token_type::I64
	);
	
	bool second_operand_is_number = (
		second_operand == token_type::I8 or second_operand == token_type::I16
		or second_operand == token_type::I32 or second_operand == token_type::I64
	);

	switch (target_operator)
	{
	case token_type::MULTIPLICATION:
	case token_type::DIVISION:
	case token_type::PLUS:
	case token_type::MINUS:
		if (first_operand_is_number and second_operand_is_number)
			return true;
		
		return false;
	
	default:
		cout << color("[Error] Operator doesn't supported", RED) << endl;
		exit(1);
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
		exit_with(interpreter_error::OPERANDS_NOT_VALID);

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
		exit_with(interpreter_error::OPERANDS_NOT_VALID);
	
	if (not are_operands_valid(op->type, first_operand->type, second_operand->type))
		exit_with(interpreter_error::OPERANDS_NOT_VALID);
	
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
