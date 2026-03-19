#ifndef ERROR_MANAGER_HPP
#define ERROR_MANAGER_HPP 

#include <cstdio>
#include <string>
#include <iostream>
#include <unordered_map>

#include "color.hpp"

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

	// Variables
	NON_EXISTENT_VARIABLE,
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
	
	interpreter_error(
		error_id id,
		string additional_info = "",
		string file_name = "",
		string file_path = "",
		size_t row = 0,
		size_t column = 0
	);
	int get_number_id();
	interpreter_error* set_position(string& file_name, string& file_path, size_t row, size_t column);
};


const string ERROR_PREFIX = "[Error]";
const string INFO_PREFIX = "[Additional informations]";
const unordered_map<error_id, string> DEFAULT_ERROR_MESSAGE = {
	{ error_id::FILE_NOT_FOUND, "File not found" },
	{ error_id::OPERANDS_NOT_VALID, "One or more operands are not valid" },
	{ error_id::NON_EXISTENT_OPERATOR, "Non existent or implemented operator" },
		{ error_id::NON_EXISTENT_VARIABLE, "Non existent variable" },
};


void exit_with(interpreter_error* err);


#endif
