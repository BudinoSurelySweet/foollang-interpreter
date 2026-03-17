#include "token.hpp"

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
		// HACK Creare un sistema per detectare le keyword
		if (context == "var")
			context_type = token_type::VAR_DECLARATION;
		else if (context == "i8")
			context_type = token_type::PRIMITIVE_TYPE;
		else if (context == "i16")
			context_type = token_type::PRIMITIVE_TYPE;
		else if (context == "i32")
			context_type = token_type::PRIMITIVE_TYPE;
		else if (context == "i64")
			context_type = token_type::PRIMITIVE_TYPE;

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


expected<bool, interpreter_error*> are_operands_valid(token* target_operator, token_type right_operand)
{
	static auto invalid_operand_err = new interpreter_error(error_id::OPERANDS_NOT_VALID);
	static auto non_existent_operator_err = new interpreter_error(error_id::NON_EXISTENT_OPERATOR);
	
	invalid_operand_err->set_position(
		target_operator->file_name,
		target_operator->file_path,
		target_operator->row,
		target_operator->column
	);

	non_existent_operator_err->set_position(
		target_operator->file_name,
		target_operator->file_path,
		target_operator->row,
		target_operator->column
	);
	
	operator_arity arity = get_operator_arity(target_operator->type);

	if (arity == operator_arity::NONE)
		return unexpected(invalid_operand_err);
	
	switch (target_operator->type)
	{
	case token_type::VAR_DECLARATION:
		if (right_operand == token_type::WORD)
			return true;
		
		return unexpected(invalid_operand_err);
	
	default:
		return unexpected(non_existent_operator_err);
	}

	return unexpected(invalid_operand_err);
}


expected<bool, interpreter_error*> are_operands_valid(token* target_operator, token_type left_operand, token_type right_operand)
{
	static auto invalid_operand_err = new interpreter_error(error_id::OPERANDS_NOT_VALID);
	static auto non_existent_operator_err = new interpreter_error(error_id::NON_EXISTENT_OPERATOR);
	
	invalid_operand_err->set_position(
		target_operator->file_name,
		target_operator->file_path,
		target_operator->row,
		target_operator->column
	);

	non_existent_operator_err->set_position(
		target_operator->file_name,
		target_operator->file_path,
		target_operator->row,
		target_operator->column
	);
	
	operator_arity arity = get_operator_arity(target_operator->type);

	if (arity == operator_arity::NONE)
		return unexpected(invalid_operand_err);

	bool left_operand_is_number = (
		left_operand == token_type::I8 or left_operand == token_type::I16
		or left_operand == token_type::I32 or left_operand == token_type::I64
	);
	
	bool right_operand_is_number = (
		right_operand == token_type::I8 or right_operand == token_type::I16
		or right_operand == token_type::I32 or right_operand == token_type::I64
	);

	switch (target_operator->type)
	{
	case token_type::MULTIPLICATION:
	case token_type::DIVISION:
	case token_type::PLUS:
	case token_type::MINUS:
		// TODO Instead of checking if they are number, I should check if they are rvalue/lvalue
		if (left_operand_is_number and right_operand_is_number)
			return true;
		
		return unexpected(invalid_operand_err);
	
	default:
		return unexpected(non_existent_operator_err);
	}

	return unexpected(invalid_operand_err);
}


int get_operator_precedence(token_type t)
{
	static const unordered_map<token_type, int> operator_precedence = {
		{ token_type::VAR_DECLARATION, 1 },
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

operands_position get_operands_position(token_type t)
{
	static const unordered_map<token_type, operands_position> positions = {
		{ token_type::VAR_DECLARATION, operands_position::RIGHT_RIGHT },
		{ token_type::MULTIPLICATION, operands_position::LEFT_RIGHT },
		{ token_type::DIVISION, operands_position::LEFT_RIGHT },
		{ token_type::REMAINDER, operands_position::LEFT_RIGHT },
		{ token_type::PLUS, operands_position::LEFT_RIGHT },
		{ token_type::MINUS, operands_position::LEFT_RIGHT },
		{ token_type::ASSIGNMENT, operands_position::LEFT_RIGHT },
		{ token_type::COMMA, operands_position::LEFT_RIGHT },
	};

	if (not is_operator(t))
		return operands_position::NONE;
	
	auto tuple = positions.find(t);
	
	if (tuple != positions.end())
		return tuple->second;
	
	return operands_position::NONE;
}


operator_arity get_operator_arity(token_type t)
{
	static const unordered_map<token_type, operator_arity> arities = {
		{ token_type::VAR_DECLARATION, operator_arity::BINARY },
		{ token_type::MULTIPLICATION, operator_arity::BINARY },
		{ token_type::DIVISION, operator_arity::BINARY },
		{ token_type::REMAINDER, operator_arity::BINARY },
		{ token_type::PLUS, operator_arity::BINARY },
		{ token_type::MINUS, operator_arity::BINARY },
		{ token_type::ASSIGNMENT, operator_arity::BINARY },
		{ token_type::COMMA, operator_arity::BINARY },
	};

	if (not is_operator(t))
		return operator_arity::NONE;
	
	auto tuple = arities.find(t);
	
	if (tuple != arities.end())
		return tuple->second;
	
	return operator_arity::NONE;
}


bool is_operator(token_type t)
{
	if (t > token_type::OPERATOR_H and t < token_type::END_OPERATOR_H)
		return true;

	return false;
}


// TODO Sostituire i due parametri così che siano coerenti con il nome della funzione: can char -> to token
bool can_char_be_in_token(token_type t, char_type c)
{
	static const set<pair<char_type, token_type>> token_to_char = {
		{ char_type::NUMBER, token_type::I8 },
		{ char_type::NUMBER, token_type::I16 },
		{ char_type::NUMBER, token_type::I32 },
		{ char_type::NUMBER, token_type::I64 },
		{ char_type::LETTER, token_type::WORD },
		{ char_type::NUMBER, token_type::WORD },
	};
	
	return token_to_char.contains({c, t});
}
