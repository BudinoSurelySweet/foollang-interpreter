#include <iostream>

#include "error_manager.hpp"
#include "color.hpp"

using namespace std;


void exit_with(interpreter_error err)
{
	int int_err = static_cast<int>(err);
	string err_prefix = "[Error]";
	string err_body = "";
	
	switch (err)
	{
	case interpreter_error::FILE_NOT_FOUND:
		err_body = "File not found";
		break;

	case interpreter_error::OPERANDS_NOT_VALID:
		err_body = "One or more operands are not valid";
		break;
	
	default:
		err_body = "No error message set for error n. " + to_string(int_err);
		break;
	}

	string error_message = err_prefix + " " + err_body;

	cout << endl << color(error_message.c_str(), RED) << endl;

	exit(int_err);
}
