#ifndef ERROR_MANAGER_HPP
#define ERROR_MANAGER_HPP 

#include <string>

using namespace std;


enum class error_id
{
	GENERIC_ERROR = 1,
	FILE_NOT_FOUND,

	// Parsing
	UNEXPECTED_TOKEN,
	
	// Operators
	OPERANDS_NOT_VALID,
	NON_EXISTENT_OPERATOR,
};


class interpreter_error
{
public:
	error_id id;
	string message;
	string info;
	string file_name;
	string file_path;
	size_t row;
	size_t column;
	
	interpreter_error(error_id id, string additional_info = "");
	int get_number_id();
};


void exit_with(interpreter_error* err);


#endif
