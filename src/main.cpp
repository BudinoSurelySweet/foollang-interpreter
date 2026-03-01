#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "interpreter.hpp"

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


int main(int argc, char** argv)
{
	// TODO Fare un sistema di argomenti migliore
	if (argc < 2)
	{
		cerr << "No file to interpret" << endl;
		
		return 1;
	}
	
	string source_code = read_file(argv[1]);
	
	interpret(&source_code);
	
	//llist<token*>* token_list = lexer(&source_code);
	
	//printf("\n\nLexing complete...\nResults are: ");
	
	//print_token_list(token_list);
	
	//lmatrix<token*>* expression_list = parser(token_list);
	
	//printf("Parser ended without errors\n");
	
	//evaluator(expression_list);
	
	return 0;
}
