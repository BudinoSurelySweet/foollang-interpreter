#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include "linked_list.hpp"

using namespace std;


static void print_tokens(linked_list<token*>* tokens)
{
	// WARNING Rimuovere questo sistema di stampa (debug only)
	cout << endl << "tokens =";

	for (list_node<token*>* t = tokens->head; t != nullptr; t = t->next)
	{
		//cout << " [\033[34;1m" << t->value->value << " (" << static_cast<int>((t->value->value).c_str()[0]) << ")" << "\033[0m]";
		cout << " [\033[34;1m" << t->value->value << "\033[0m]";

		if (t->next != nullptr)
			cout << ", ";
		else
			cout << endl;
	}
}


// WARNING Mettere questa funzione dentro il parser
linked_list<list_node<linked_list<list_node<token*>*>*>*>* operator_precedence(linked_list<token*>* tokens)
{
	linked_list<list_node<linked_list<list_node<token*>*>*>*>* operators = new linked_list<list_node<linked_list<list_node<token*>*>*>*>;
	
	size_t operator_count = 2;

	for (size_t i = 0; i < operator_count; i++)
	{
		list_node<linked_list<list_node<token*>*>*>* temp = new list_node<linked_list<list_node<token*>*>*>;
		operators->push(temp);
	}
	
	for (list_node<token*>* curr_token = tokens->head; curr_token != nullptr; curr_token = curr_token->next)
	{
		// WARNING (convertirlo in inglese) Più la precedenza è alta e meno l'operatore ha precedenza.
		u_int8_t precedence_value = 0;
		token_type tok_t = curr_token->value->type;

		if (tok_t == token_type::OPERATOR_MULTIPLICATION or tok_t == token_type::OPERATOR_DIVISION)
			precedence_value = 1;

		if (tok_t == token_type::OPERATOR_PLUS or tok_t == token_type::OPERATOR_MINUS)
			precedence_value = 2;
		
		list_node<linked_list<list_node<token*>*>*>* curr_linked_list = operators->head;

		for (size_t i = 0; i < precedence_value; i++, curr_linked_list = curr_linked_list->next)
		{
			if (i + 1 == precedence_value)
			{
				curr_linked_list->value->push(curr_token);
			}
		}
	}

	return operators;
}


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cerr << "No file to interpret" << endl;
		
		return 1;
	}
	
	printf("\nInterpreter started.\n");
	
	ifstream file(argv[1]);
	stringstream buffer;
	buffer << file.rdbuf();
	string source_code = buffer.str();
	
	file.close();
	
	linked_list<token*>* tokens = lexer(&source_code);
	
	print_tokens(tokens);
	
	operator_precedence(tokens);

	return 0;
}
