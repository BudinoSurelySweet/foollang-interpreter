#include <iostream>

#include "evaluator.hpp"

using namespace std;


void evaluate_expressions(linked_list<ast_expression*>* expressions)
{
	for (list_node<ast_expression*>* exp = expressions->head; exp != nullptr; exp = exp->next)
	{
		cout << exp->value->evaluate() << endl;
	}
}
