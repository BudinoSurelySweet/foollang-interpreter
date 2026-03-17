#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <unordered_map>
#include <optional>
#include <expected>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <set>

#include "error_manager.hpp"
#include "color.hpp"

#define OPERATORS_LEVELS_OF_PRECEDENCE_NUMBER 14

using namespace std;


enum class char_type
{
	NONE,
	
	// TODO Aggiungere il supporto per i caratteri da 0 a 31 (saranno da ignorare nella fase di lexing) + 32 (cioè lo spazio)
	
	// TODO Altri carateri saranno da gestire con errori (unexpected_token)

	SPACE = ' ',

	// Group of characters
	NUMBER = '0', // 0-9
	LETTER = 'a', // a-z A-Z

	// ...
	PLUS = '+',
	HYPHEN = '-',
	ASTERISK = '*',
	SLASH = '/',
	EQUALS = '=',
	TILDE = '~',
	AMPERSAND = '&',
	PERCENTAGE = '%',

	// Punctuations
	COMMA = ',',
	COLON = ':',
	SEMICOLON = ';',

	// Brackets
	LEFT_PARENTHESIS = '(',
	RIGHT_PARENTHESIS = ')',
	LEFT_SQUARE_BRACKET = '[',
	RIGHT_SQUARE_BRACKET = ']',
	LEFT_CURLY_BRACKET = '{',
	RIGHT_CURLY_BRACKET = '}',
	LEFT_ANGLED_BRACKET = '<',
	RIGHT_ANGLED_BRACKET = '>',
};


enum class token_type
{
	NONE,
	TOMBSTONE, // Used by the evaluator

	WORD, // [a-zA-Z][a-zA-Z0-9]* <- avarage regex enjoyer
	PRIMITIVE_TYPE,
	
	NUMBERS_H,

		I8, // 8 bit - 1 bytes
		I16, // 16 bit - 2 bytes
		I32, // 32 bit - 4 bytes
		I64, // 64 bit - 8 bytes

	END_NUMBERS_H,

	OPERATOR_H,

		VAR_DECLARATION,
		MULTIPLICATION,
		DIVISION,
		REMAINDER,
		PLUS,
		MINUS,
		ASSIGNMENT,
		COMMA,

	END_OPERATOR_H,

	PUNCTUATION_H,

		SEQUENCE_POINT,
		LEFT_GROUP_LIMITER,
		RIGHT_GROUP_LIMITER,

	END_PUNCTUATION_H,
};


enum class operator_associativity
{
	NONE,
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT
};


enum class operands_position
{
	NONE,
	LEFT,
	RIGHT,
	LEFT_RIGHT,
	LEFT_LEFT,
	RIGHT_RIGHT,
};


enum class operator_arity
{
	NONE,
	UNARY,
	BINARY,
};


class token
{
public:
	token_type type;
	string lexeme;
	
	// HACK Storing strings directly inside tokens for now. Since strings allocate their buffer on the heap, 1 million tokens on the stack results in 2 million heap allocations.	string file_name;
	string file_name;
	string file_path;
	size_t row;
	size_t column;
	
	token(token_type new_type, string new_lexeme, string new_file_name, string new_file_path, size_t new_row, size_t new_column);
};


optional<token> create_token(char character, string target_file_name, string target_file_path, size_t target_row, size_t target_column);


char_type get_char_type(char c);


token_type get_token_type(char_type c);


expected<bool, interpreter_error*> are_operands_valid(token* target_operator, token_type right_operand);


expected<bool, interpreter_error*> are_operands_valid(token* target_operator, token_type left_operand, token_type right_operand);


// TODO Sostituire tutte le funzioni sottostanti con `get_trait`
operator_arity get_trait(token_type t, unordered_map<token_type, operator_arity> map);
operands_position get_trait(token_type t, unordered_map<token_type, operands_position> map);


int get_operator_precedence(token_type t);


operator_arity get_operator_associativity(char c);


operands_position get_operands_position(token_type t);


operator_arity get_operator_arity(token_type t);


bool is_operator(token_type t);


bool can_char_be_in_token(token_type t, char_type c);


#endif
