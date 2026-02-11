#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include "linked_list.hpp"

using namespace std;


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
	
	linked_list<token*>* token_list = lexer(&source_code);
	
	// WARNING Rimuovere questo sistema di stampa (debug only)
	cout << endl << "tokens =";

	for (list_node<token*>* t = token_list->head; t != nullptr; t = t->next)
	{
		//cout << " [\033[34;1m" << t->value->value << " (" << static_cast<int>((t->value->value).c_str()[0]) << ")" << "\033[0m]";
		cout << " [\033[34;1m" << t->value->value << "\033[0m]";

		if (t->next != nullptr)
			cout << ", ";
		else
			cout << endl;
	}

	linked_list<ast_expression*>* expressions = parser(token_list);
	
	evaluate_expressions(expressions);

	return 0;
}
