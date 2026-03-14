#ifndef ERROR_MANAGER_HPP
#define ERROR_MANAGER_HPP 

enum class interpreter_error
{
	GENERIC_ERROR = 1,
	FILE_NOT_FOUND,

	// Parsing
	UNEXPECTED_TOKEN,
	
	// Operators
	OPERANDS_NOT_VALID,
};

void exit_with(interpreter_error err);

#endif
