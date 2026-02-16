#include "evaluator.hpp"


static void evaluate_expression(llist<token*>* exp)
{
	const size_t OPERATORS_PRECEDENCE_LEVELS = 2;
	
	llist<token*> operators[OPERATORS_PRECEDENCE_LEVELS];
	
	// TODO Scrivere la logica di valutazione dell'espressione
}


void evaluator(lmatrix<token*>* expression_list)
{
	for (
			lnode<llist<token*>*>* curr_node = expression_list->head;
			curr_node != nullptr;
			curr_node = curr_node->next
	)
	{
		llist<token*>* curr_expression = curr_node->value;
		evaluate_expression(curr_expression);
	}
}
