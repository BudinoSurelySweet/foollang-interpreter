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


void token::become_tombstone()
{
	lexeme.clear();
	type = token_type::TOMBSTONE;
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
	if (can_char_be_in_token(character_type, context_type))
	{
		context.push_back(character);
		
		return nullopt;
	}
	else
	{
		// HACK: Creare un sistema per detectare le keyword
		if (context == "var")
			context_type = token_type::VAR_DECLARATION;

		if (is_primitive_lang_type(context))
			context_type = token_type::PRIMITIVE_LANG_TYPE;

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
	auto tuple = INT_TO_CHAR_MAP.find(tolower(c));
	
	if (tuple != INT_TO_CHAR_MAP.end())
		return tuple->second;
	
	return char_type::NONE;
}


token_type get_token_type(char_type c)
{
	auto tuple = CHAR_TO_TOKEN_MAP.find(c);
	
	if (tuple != CHAR_TO_TOKEN_MAP.end())
		return tuple->second;
	
	return token_type::NONE;
}


int get_operator_precedence(token_type t)
{
	auto tuple = OPERATOR_PRECEDENCE_MAP.find(t);
	
	if (tuple != OPERATOR_PRECEDENCE_MAP.end())
		return tuple->second;
	
	return -1;
}


operands_position get_operands_position(token_type t)
{
	if (not is_operator(t))
		return operands_position::NONE;
	
	auto tuple = OPERATION_OPERANDS_POSITION_MAP.find(t);
	
	if (tuple != OPERATION_OPERANDS_POSITION_MAP.end())
		return tuple->second;
	
	return operands_position::NONE;
}


operator_arity get_operator_arity(token_type t)
{
	if (not is_operator(t))
		return operator_arity::NONE;
	
	auto tuple = OPERATOR_ARITY_MAP.find(t);
	
	if (tuple != OPERATOR_ARITY_MAP.end())
		return tuple->second;
	
	return operator_arity::NONE;
}


value_access_type get_value_access_type(token_type t)
{
	auto tuple = VALUE_ACCESS_TYPE_MAP.find(t);

	if (tuple != VALUE_ACCESS_TYPE_MAP.end())
		return tuple->second;

	return value_access_type::NONE;
}


bool is_operator(token_type t)
{
	if (t > token_type::OPERATOR_H and t < token_type::END_OPERATOR_H)
		return true;

	return false;
}


bool is_primitive_lang_type(string t)
{
	auto tuple = PRIMITIVE_LANG_TYPE_KEYWORDS.find(t);

	if (tuple != PRIMITIVE_LANG_TYPE_KEYWORDS.end())
		return true;

	return false;
}


bool can_char_be_in_token(char_type c, token_type t)
{
	return CHAR_TOKEN_AVAILABLE_PAIRS.contains({c, t});
}

