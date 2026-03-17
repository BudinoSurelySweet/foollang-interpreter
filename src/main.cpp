#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "interpreter.hpp"
#include "error_manager.hpp"
#include "color.hpp"

using namespace std;


static string read_file(string file_path)
{
	ifstream file(file_path);
	stringstream buffer;
	
	if (not file)
	{
		auto err = new interpreter_error(error_id::FILE_NOT_FOUND);

		exit_with(err);
	}

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
		auto err = new interpreter_error(error_id::FILE_NOT_FOUND);
		
		exit_with(err);
	}
	
	string file_name = argv[1];
	string file_path = filesystem::absolute(argv[1]).string();
	
	string source_code = read_file(file_name);
	
	interpret(source_code, file_name, file_path);

	return 0;
}
