#include <iostream>

#include "token.hpp"
#include "color.hpp"

using namespace std;


token::token(token_type new_type, string new_lexeme)
{
	type = new_type;
	lexeme = new_lexeme;
}


// HACK Provare ad automatizzare questo processo
char_type get_char_type(char c)
{
	bool is_number = '0' <= c and c <= '9';
	bool is_letter = ('a' <= c and c <= '<') or ('A' <= c and c <= 'Z');
	
	if (is_number)
		return char_type::NUMBER;

	if (is_letter)
		return char_type::LETTER;

	switch (c)
	{
	case (int) char_type::PLUS: return char_type::PLUS;
	case (int) char_type::HYPHEN: return char_type::HYPHEN;
	case (int) char_type::ASTERISK: return char_type::ASTERISK;
	case (int) char_type::SLASH: return char_type::SLASH;
	case (int) char_type::EQUALS: return char_type::EQUALS;
	case (int) char_type::TILDE: return char_type::TILDE;
	case (int) char_type::AMPERSAND: return char_type::AMPERSAND;
	case (int) char_type::PERCENTAGE: return char_type::PERCENTAGE;
	case (int) char_type::COMMA: return char_type::COMMA;
	case (int) char_type::COLON: return char_type::COLON;
	case (int) char_type::SEMICOLON: return char_type::SEMICOLON;
	case (int) char_type::LEFT_PARENTHESIS: return char_type::LEFT_PARENTHESIS;
	case (int) char_type::RIGHT_PARENTHESIS: return char_type::RIGHT_PARENTHESIS;
	case (int) char_type::LEFT_SQUARE_BRACKET: return char_type::LEFT_SQUARE_BRACKET;
	case (int) char_type::RIGHT_SQUARE_BRACKET: return char_type::RIGHT_SQUARE_BRACKET;
	case (int) char_type::LEFT_CURLY_BRACKET: return char_type::LEFT_CURLY_BRACKET;
	case (int) char_type::RIGHT_CURLY_BRACKET: return char_type::RIGHT_CURLY_BRACKET;
	case (int) char_type::LEFT_ANGLED_BRACKET: return char_type::LEFT_ANGLED_BRACKET;
	case (int) char_type::RIGHT_ANGLED_BRACKET: return char_type::RIGHT_ANGLED_BRACKET;
	
	default: return char_type::NONE;
	}
}


// HACK Provare ad automatizzare questo processo
token_type get_token_type(char_type c)
{
	switch (c)
	{
	case char_type::NUMBER: return token_type::I32;
	case char_type::LETTER: return token_type::WORD;
	case char_type::PLUS: return token_type::PLUS;
	case char_type::HYPHEN: return token_type::MINUS;
	case char_type::ASTERISK: return token_type::MULTIPLICATION;
	case char_type::SLASH: return token_type::DIVISION;
	case char_type::EQUALS: return token_type::ASSIGNMENT;
	case char_type::PERCENTAGE: return token_type::REMAINDER;
	case char_type::COMMA: return token_type::COMMA;
	case char_type::SEMICOLON: return token_type::SEQUENCE_POINT;
	
	default:
		return token_type::NONE;
		break;
	}
}


token_type get_operator_precedence(token_type t)
{
	if (t > token_type::PRECEDENCE_LEVEL_1 and t < token_type::PRECEDENCE_LEVEL_2)
		return token_type::PRECEDENCE_LEVEL_1;

	if (t > token_type::PRECEDENCE_LEVEL_2 and t < token_type::PRECEDENCE_LEVEL_3)
		return token_type::PRECEDENCE_LEVEL_2;
	
	if (t > token_type::PRECEDENCE_LEVEL_3 and t < token_type::PRECEDENCE_LEVEL_4)
		return token_type::PRECEDENCE_LEVEL_3;
	
	if (t > token_type::PRECEDENCE_LEVEL_4 and t < token_type::PRECEDENCE_LEVEL_5)
		return token_type::PRECEDENCE_LEVEL_4;
	
	if (t > token_type::PRECEDENCE_LEVEL_5 and t < token_type::PRECEDENCE_LEVEL_6)
		return token_type::PRECEDENCE_LEVEL_5;
	
	if (t > token_type::PRECEDENCE_LEVEL_6 and t < token_type::PRECEDENCE_LEVEL_7)
		return token_type::PRECEDENCE_LEVEL_6;
	
	if (t > token_type::PRECEDENCE_LEVEL_7 and t < token_type::PRECEDENCE_LEVEL_8)
		return token_type::PRECEDENCE_LEVEL_7;
	
	if (t > token_type::PRECEDENCE_LEVEL_8 and t < token_type::PRECEDENCE_LEVEL_9)
		return token_type::PRECEDENCE_LEVEL_8;
	
	if (t > token_type::PRECEDENCE_LEVEL_9 and t < token_type::PRECEDENCE_LEVEL_10)
		return token_type::PRECEDENCE_LEVEL_9;
	
	if (t > token_type::PRECEDENCE_LEVEL_10 and t < token_type::PRECEDENCE_LEVEL_11)
		return token_type::PRECEDENCE_LEVEL_10;
	
	if (t > token_type::PRECEDENCE_LEVEL_11 and t < token_type::PRECEDENCE_LEVEL_12)
		return token_type::PRECEDENCE_LEVEL_11;
	
	if (t > token_type::PRECEDENCE_LEVEL_12 and t < token_type::PRECEDENCE_LEVEL_13)
		return token_type::PRECEDENCE_LEVEL_12;
	
	if (t > token_type::PRECEDENCE_LEVEL_13 and t < token_type::PRECEDENCE_LEVEL_14)
		return token_type::PRECEDENCE_LEVEL_13;
	
	if (t > token_type::PRECEDENCE_LEVEL_14 and t < token_type::END_OPERATOR_H)
		return token_type::PRECEDENCE_LEVEL_14;

	return token_type::NONE;
}


int operator_precedence_to_int(token_type t)
{
	if (t > token_type::PRECEDENCE_LEVEL_1 and t < token_type::PRECEDENCE_LEVEL_2)
		return 1;

	if (t > token_type::PRECEDENCE_LEVEL_2 and t < token_type::PRECEDENCE_LEVEL_3)
		return 2;
	
	if (t > token_type::PRECEDENCE_LEVEL_3 and t < token_type::PRECEDENCE_LEVEL_4)
		return 3;
	
	if (t > token_type::PRECEDENCE_LEVEL_4 and t < token_type::PRECEDENCE_LEVEL_5)
		return 4;
	
	if (t > token_type::PRECEDENCE_LEVEL_5 and t < token_type::PRECEDENCE_LEVEL_6)
		return 5;
	
	if (t > token_type::PRECEDENCE_LEVEL_6 and t < token_type::PRECEDENCE_LEVEL_7)
		return 6;
	
	if (t > token_type::PRECEDENCE_LEVEL_7 and t < token_type::PRECEDENCE_LEVEL_8)
		return 7;
	
	if (t > token_type::PRECEDENCE_LEVEL_8 and t < token_type::PRECEDENCE_LEVEL_9)
		return 8;
	
	if (t > token_type::PRECEDENCE_LEVEL_9 and t < token_type::PRECEDENCE_LEVEL_10)
		return 9;
	
	if (t > token_type::PRECEDENCE_LEVEL_10 and t < token_type::PRECEDENCE_LEVEL_11)
		return 10;
	
	if (t > token_type::PRECEDENCE_LEVEL_11 and t < token_type::PRECEDENCE_LEVEL_12)
		return 11;
	
	if (t > token_type::PRECEDENCE_LEVEL_12 and t < token_type::PRECEDENCE_LEVEL_13)
		return 12;
	
	if (t > token_type::PRECEDENCE_LEVEL_13 and t < token_type::PRECEDENCE_LEVEL_14)
		return 13;
	
	if (t > token_type::PRECEDENCE_LEVEL_14 and t < token_type::END_OPERATOR_H)
		return 14;

	return -1;
}


//bool are_operands_valid(token_type target_operator, token_type first_operand)
//{
//	operator_arity arity = get_operator_arity(target_operator);
	
//	if (arity == operator_arity::NONE or arity != operator_arity::UNARY)
//		return false;
//}


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
		break;
	}

	return false;
}


//bool are_operands_valid(token_type target_operator, token_type first_operand, token_type second_operand, token_type third_operand)
//{
//	operator_arity arity = get_operator_arity(target_operator);
	
//	if (arity == operator_arity::NONE or arity != operator_arity::TERNARY)
//		return false;
//}


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


void evaluate_operands(llist<token*>* list, token* op, lnode<token*>* first_operand, lnode<token*>* second_operand)
{
	// TODO Create integer promotion
	// ...
	
	auto left_number = atoi(first_operand->value->lexeme.c_str());
	auto right_number = atoi(second_operand->value->lexeme.c_str());

	switch (op->type)
	{
	case token_type::PLUS:
		op->lexeme = to_string(left_number + right_number);
		op->type = token_type::I32;
		
		list->remove_node(first_operand);
		list->remove_node(second_operand);

		break;
	
	case token_type::MINUS:
		op->lexeme = to_string(left_number - right_number);
		op->type = token_type::I32;

		list->remove_node(first_operand);
		list->remove_node(second_operand);
		
		break;
	
	case token_type::MULTIPLICATION:
		op->lexeme = to_string(left_number * right_number);
		op->type = token_type::I32;

		list->remove_node(first_operand);
		list->remove_node(second_operand);
		
		break;

	case token_type::DIVISION:
		op->lexeme = to_string(left_number / right_number);
		op->type = token_type::I32;

		list->remove_node(first_operand);
		list->remove_node(second_operand);
		
		break;
	
	default:
		break;
	}
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
