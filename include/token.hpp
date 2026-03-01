#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

using namespace std;


enum class char_type
{
	NONE,

	// Group of characters
	NUMBER, // 0-9
	LETTER, // a-z A-Z

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
	
	NUMBERS_H,

		I8, // 8 bit - 1 bytes
		I16, // 16 bit - 2 bytes
		I32, // 32 bit - 4 bytes
		I64, // 64 bit - 8 bytes

	OPERATOR_H,

		PRECEDENCE_LEVEL_1,
		PRECEDENCE_LEVEL_2,
		PRECEDENCE_LEVEL_3,

			MULTIPLICATION,
			DIVISION,
			REMAINDER,

		PRECEDENCE_LEVEL_4,

			PLUS,
			MINUS,

		PRECEDENCE_LEVEL_5,
		PRECEDENCE_LEVEL_6,
		PRECEDENCE_LEVEL_7,
		PRECEDENCE_LEVEL_8,
		PRECEDENCE_LEVEL_9,
		PRECEDENCE_LEVEL_10,
		PRECEDENCE_LEVEL_11,
		PRECEDENCE_LEVEL_12,
		PRECEDENCE_LEVEL_13,
		
			ASSIGNMENT,
			
		PRECEDENCE_LEVEL_14,

			COMMA,

	PUNCTUATION_H,

		SEQUENCE_POINT,
		LEFT_GROUP_LIMITER,
		RIGHT_GROUP_LIMITER,
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


struct token
{
private:
	token_type type;
	string* lexeme;
	
public:
	token(token_type new_type, string* new_lexeme);
};


char_type get_char_type(char c);


token_type get_operator_precedence(token_type t);


operator_arity get_operator_associativity(char c);


operator_arity get_operator_arity(char c);


#endif
