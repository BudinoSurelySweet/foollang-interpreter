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
	void become_tombstone();
};


#pragma region Token data

// The uppercase case counterpart is managed in the `get_char_type` function.
static const unordered_map<int, char_type> INT_TO_CHAR_MAP = {
	{ '0', char_type::NUMBER },
	{ '1', char_type::NUMBER },
	{ '2', char_type::NUMBER },
	{ '3', char_type::NUMBER },
	{ '4', char_type::NUMBER },
	{ '5', char_type::NUMBER },
	{ '6', char_type::NUMBER },
	{ '7', char_type::NUMBER },
	{ '8', char_type::NUMBER },
	{ '9', char_type::NUMBER },
	{ 'a', char_type::LETTER },
	{ 'b', char_type::LETTER },
	{ 'c', char_type::LETTER },
	{ 'd', char_type::LETTER },
	{ 'e', char_type::LETTER },
	{ 'f', char_type::LETTER },
	{ 'g', char_type::LETTER },
	{ 'h', char_type::LETTER },
	{ 'i', char_type::LETTER },
	{ 'j', char_type::LETTER },
	{ 'k', char_type::LETTER },
	{ 'l', char_type::LETTER },
	{ 'm', char_type::LETTER },
	{ 'n', char_type::LETTER },
	{ 'o', char_type::LETTER },
	{ 'p', char_type::LETTER },
	{ 'q', char_type::LETTER },
	{ 'r', char_type::LETTER },
	{ 's', char_type::LETTER },
	{ 't', char_type::LETTER },
	{ 'u', char_type::LETTER },
	{ 'v', char_type::LETTER },
	{ 'w', char_type::LETTER },
	{ 'x', char_type::LETTER },
	{ 'y', char_type::LETTER },
	{ 'z', char_type::LETTER },
	{ static_cast<int>(char_type::PLUS), char_type::PLUS },
	{ static_cast<int>(char_type::HYPHEN), char_type::HYPHEN },
	{ static_cast<int>(char_type::ASTERISK), char_type::ASTERISK },
	{ static_cast<int>(char_type::SLASH), char_type::SLASH },
	{ static_cast<int>(char_type::EQUALS), char_type::EQUALS },
	{ static_cast<int>(char_type::TILDE), char_type::TILDE },
	{ static_cast<int>(char_type::AMPERSAND), char_type::AMPERSAND },
	{ static_cast<int>(char_type::PERCENTAGE), char_type::PERCENTAGE },
	{ static_cast<int>(char_type::COMMA), char_type::COMMA },
	{ static_cast<int>(char_type::COLON), char_type::COLON },
	{ static_cast<int>(char_type::SEMICOLON), char_type::SEMICOLON },
	{ static_cast<int>(char_type::LEFT_PARENTHESIS), char_type::LEFT_PARENTHESIS },
	{ static_cast<int>(char_type::RIGHT_PARENTHESIS), char_type::RIGHT_PARENTHESIS },
	{ static_cast<int>(char_type::LEFT_SQUARE_BRACKET), char_type::LEFT_SQUARE_BRACKET },
	{ static_cast<int>(char_type::RIGHT_SQUARE_BRACKET), char_type::RIGHT_SQUARE_BRACKET },
	{ static_cast<int>(char_type::LEFT_CURLY_BRACKET), char_type::LEFT_CURLY_BRACKET },
	{ static_cast<int>(char_type::RIGHT_CURLY_BRACKET), char_type::RIGHT_CURLY_BRACKET },
	{ static_cast<int>(char_type::LEFT_ANGLED_BRACKET), char_type::LEFT_ANGLED_BRACKET },
	{ static_cast<int>(char_type::RIGHT_ANGLED_BRACKET), char_type::RIGHT_ANGLED_BRACKET },
};


static const unordered_map<char_type, token_type> CHAR_TO_TOKEN_MAP = {
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


static const unordered_map<token_type, int> OPERATOR_PRECEDENCE_MAP = {
	{ token_type::VAR_DECLARATION, 1 },
	{ token_type::MULTIPLICATION, 3 },
	{ token_type::DIVISION, 3 },
	{ token_type::REMAINDER, 3 },
	{ token_type::PLUS, 4 },
	{ token_type::MINUS, 4 },
	{ token_type::ASSIGNMENT, 13 },
	{ token_type::COMMA, 14 },
};


static const unordered_map<token_type, operands_position> OPERATION_OPERANDS_POSITION_MAP = {
	{ token_type::VAR_DECLARATION, operands_position::RIGHT_RIGHT },
	{ token_type::MULTIPLICATION, operands_position::LEFT_RIGHT },
	{ token_type::DIVISION, operands_position::LEFT_RIGHT },
	{ token_type::REMAINDER, operands_position::LEFT_RIGHT },
	{ token_type::PLUS, operands_position::LEFT_RIGHT },
	{ token_type::MINUS, operands_position::LEFT_RIGHT },
	{ token_type::ASSIGNMENT, operands_position::LEFT_RIGHT },
	{ token_type::COMMA, operands_position::LEFT_RIGHT },
};


static const unordered_map<token_type, operator_arity> OPERATOR_ARITY_MAP = {
	{ token_type::VAR_DECLARATION, operator_arity::BINARY },
	{ token_type::MULTIPLICATION, operator_arity::BINARY },
	{ token_type::DIVISION, operator_arity::BINARY },
	{ token_type::REMAINDER, operator_arity::BINARY },
	{ token_type::PLUS, operator_arity::BINARY },
	{ token_type::MINUS, operator_arity::BINARY },
	{ token_type::ASSIGNMENT, operator_arity::BINARY },
	{ token_type::COMMA, operator_arity::BINARY },
};


static const set<pair<char_type, token_type>> CHAR_TOKEN_AVAILABLE_PAIRS = {
	{ char_type::NUMBER, token_type::I8 },
	{ char_type::NUMBER, token_type::I16 },
	{ char_type::NUMBER, token_type::I32 },
	{ char_type::NUMBER, token_type::I64 },
	{ char_type::LETTER, token_type::WORD },
	{ char_type::NUMBER, token_type::WORD },
};

#pragma endregion


optional<token> create_token(char character, string target_file_name, string target_file_path, size_t target_row, size_t target_column);


#pragma region Get token traits

char_type get_char_type(char c);


token_type get_token_type(char_type c);


int get_operator_precedence(token_type t);


operands_position get_operands_position(token_type t);


operator_arity get_operator_arity(token_type t);


bool is_operator(token_type t);


bool can_char_be_in_token(char_type c, token_type t);

#pragma endregion


#endif
