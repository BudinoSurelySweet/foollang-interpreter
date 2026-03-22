#pragma once

#include <cstdio>
#include <string>
#include <unordered_map>

using namespace std;


// Forward declarations
class token;


enum class error_id
{
	GENERIC_ERROR = 1,
	FILE_NOT_FOUND,

	// Parsing
	UNEXPECTED_TOKEN,
	INTEGER_OVERFLOW,
	
	// Operators
	OPERANDS_NOT_VALID,
	NON_EXISTENT_OPERATOR,

	// Variables
	NON_EXISTENT_LANG_TYPE,
	NON_EXISTENT_VARIABLE,
	DUPLICATE_VARIABLE,
};


class interpreter_error
{
private:
	error_id id;
	string message;
	string info;

public:
	string file_name;
	string file_path;
	size_t row;
	size_t column;

	interpreter_error(
		error_id id,
		string info = "",
		string file_name = "",
		string file_path = "",
		size_t row = 0,
		size_t column = 0
	);
	interpreter_error(
		error_id id,
		token* fault_target,
		string info = ""
	);
	int get_number_id();
	interpreter_error* set_position(string& file_name, string& file_path, size_t row, size_t column);
	interpreter_error* set_position(token* fault_target);

	interpreter_error* set_message(const string &message);
	interpreter_error* set_info(const string &info);

	string get_message();
	string get_info();
};


const string ERROR_PREFIX = "[Error]";
const string INFO_PREFIX = "[Additional informations]";
const unordered_map<error_id, string> DEFAULT_ERROR_MESSAGE = {
	{ error_id::FILE_NOT_FOUND, "File not found" },
	{ error_id::OPERANDS_NOT_VALID, "One or more operands are not valid" },
	{ error_id::NON_EXISTENT_OPERATOR, "Non existent or implemented operator" },
	{ error_id::NON_EXISTENT_VARIABLE, "Non existent variable" },
	{ error_id::DUPLICATE_VARIABLE, "Variable with the same name already exists" },
	{ error_id::INTEGER_OVERFLOW, "The number inserted can't be represented by primitive numbers" },
};


void exit_with(interpreter_error* err);

