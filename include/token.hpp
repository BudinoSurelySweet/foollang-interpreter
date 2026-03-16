#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <optional>
#include <vector>
#include <string>

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
	
	NUMBERS_H,

		I8, // 8 bit - 1 bytes
		I16, // 16 bit - 2 bytes
		I32, // 32 bit - 4 bytes
		I64, // 64 bit - 8 bytes

	END_NUMBERS_H,

	OPERATOR_H,

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


enum class operator_arity
{
	NONE,
	UNARY,
	BINARY,
	TERNARY,
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


int get_operator_precedence(token_type t);


bool are_operands_valid(token_type target_operator, token_type first_operand);


bool are_operands_valid(token* target_operator, token_type first_operand, token_type second_operand);


bool are_operands_valid(token_type target_operator, token_type first_operand, token_type second_operand, token_type third_operand);


operator_arity get_operator_associativity(char c);


operator_arity get_operator_arity(token_type t);


void evaluate_operands(vector<token>* tokens, token* op, size_t pos);


bool is_operator(token_type t);


bool can_char_be_in_token(token_type t, char_type c);


#endif
