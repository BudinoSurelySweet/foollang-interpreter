#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"

using namespace std;


static string read_file(char* file_path)
{
	ifstream file(file_path);
	stringstream buffer;

	buffer << file.rdbuf();

	string result = buffer.str();
	
	file.close();

	return result;
}


// WARNING Rimuovere questo sistema di stampa (debug only)
static void print_token_list(llist<token*>* token_list)
{
	for (lnode<token*>* t = token_list->head; t != nullptr; t = t->next)
	{
		//cout << " [\033[34;1m" << t->value->value << " (" << static_cast<int>((t->value->value).c_str()[0]) << ")" << "\033[0m]";
		cout << "[\033[34;1m" << t->value->value << "\033[0m]";
		
		if (t->next != nullptr)
			cout << ", ";
		else
			cout << endl;
	}
}


int main(int argc, char** argv)
{
	// TODO Fare un sistema di argomenti migliore
	if (argc < 2)
	{
		cerr << "No file to interpret" << endl;
		
		return 1;
	}
	
	string source_code = read_file(argv[1]);
	
	llist<token*>* token_list = lexer(&source_code);
	
	printf("\n\nLexing complete...\nResults are: ");
	
	print_token_list(token_list);
	
	lmatrix<token*>* expression_list = parser(token_list);
	
	printf("Parser ended without errors\n");
	
	evaluator(expression_list);
	
	return 0;
}
