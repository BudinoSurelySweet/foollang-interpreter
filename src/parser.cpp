#include "parser.hpp"


lmatrix<token*>* parser(llist<token*>* token_list)
{
	lmatrix<token*>* expression_list = new lmatrix<token*>;
	llist<token*>* curr_expression = new llist<token*>;

	for (lnode<token*>* curr_node = token_list->head; curr_node != nullptr; curr_node = curr_node->next)
	{
		token* curr_token = curr_node->value;
		
		// TODO Controllare la validità dell'espressione

		if (curr_token->type == token_type::SEQUENCE_POINT)
		{
			expression_list->append(curr_expression);
			curr_expression->clear();

			continue;
		}
		
		curr_expression->append(curr_token);
	}
	
	return expression_list;
}
