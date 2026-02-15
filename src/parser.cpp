#include <iostream>

#include "parser.hpp"
#include "diagnostics.hpp"

using namespace std;


token_type ast_base::get_type() const
{
	return type;
}


ast_derivated::ast_derivated(ast_base* new_left, ast_base* new_right)
{
	left = new_left;
	right = new_right;
	is_primitive = false;
}


void ast_derivated::evaluate()
{
	if (not left->is_primitive) static_cast<ast_derivated*>(left)->evaluate();
	if (not right->is_primitive) static_cast<ast_derivated*>(right)->evaluate();
}


ast_derivated::~ast_derivated()
{
	if (left) delete left;
	if (right) delete right;
}


ast_i32::ast_i32(int32_t new_value) : ast_primitive<int32_t>(new_value) {
	type = token_type::I32;
	value = new_value;
}


ast_operator_plus::ast_operator_plus()
{
	type = token_type::OPERATOR_PLUS;
	is_primitive = false;
}


void ast_operator_plus::evaluate()
{
	ast_derivated::evaluate();

	ast_i32* left_i32 = static_cast<ast_i32*>(left);
	ast_i32* right_i32 = static_cast<ast_i32*>(right);
	
	int sum = left_i32->evaluate() + right_i32->evaluate();
	
	associated_primitive = new ast_i32(sum);
	
	cout << "SUM: " << sum << endl;
	
	this->~ast_operator_plus();
}


ast_expression::ast_expression(linked_list<token*>* linear_expression)
{
	// WARNING Rimuovere questo sistema di stampa (debug only).
	printf("Expression =");

	for (list_node<token*>* t = linear_expression->head; t != nullptr; t = t->next)
	{
		cout << " [\033[34;1m" << t->value->value << "\033[0m]";

		if (t->next != nullptr)
			cout << ", ";
		else
			cout << endl;
	}
	
	// TODO Creare l'espressione

	for (list_node<token*>* token = linear_expression->head; token != nullptr; token = token->next)
	{
		if (token->value->type == token_type::OPERATOR_PLUS)
		{
			ast_i32* left = new ast_i32(stoi(token->prev->value->value));
			ast_i32* right = new ast_i32(stoi(token->next->value->value));

			ast_derivated* node = new ast_derivated(left, right);
			
			down = node;
		}
		
	}
}


// WARNING Pensare a cosa restituire dalla expressione (restituire un int come in C?)
int32_t ast_expression::evaluate()
{
	if (down->is_primitive)
	{
		switch (type)
		{
			case token_type::I32:
				return static_cast<ast_i32*>(down)->evaluate();
			default:
				return -1;
		}
	}
	else
	{
		static_cast<ast_derivated*>(down)->evaluate();
		
		return static_cast<ast_i32*>(static_cast<ast_derivated*>(down)->associated_primitive)->value;
	}
}


linked_list<ast_expression*>* parser(linked_list<token*>* token_list)
{
	linked_list<ast_expression*>* expressions = new linked_list<ast_expression*>;
	linked_list<token*>* linear_expression = new linked_list<token*>;
	
	for (list_node<token*>* t = token_list->head; t != nullptr; t = t->next)
	{
		if (t->value->type == token_type::SEQUENCE_POINT)
		{
			ast_expression* e = new ast_expression(linear_expression);

			expressions->append(e);
			linear_expression->clear();
		}
		else
		{
			token* new_token = new token(t->value->type, t->value->value, nullptr);
			
			linear_expression->append(new_token);
		}
	}
	
	return expressions;
}
