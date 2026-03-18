#include <iostream>
#include <unordered_map>

#include "error_manager.hpp"
#include "color.hpp"

using namespace std;


interpreter_error::interpreter_error(error_id id, string additional_info, string file_name, string file_path, size_t row, size_t column)
{
	static const string err_prefix = "[Error]";
	static const string info_prefix = "[Additional informations]";
	static const unordered_map<error_id, string> default_message = {
		{ error_id::FILE_NOT_FOUND, "File not found" },
		{ error_id::OPERANDS_NOT_VALID, "One or more operands are not valid" },
		{ error_id::NON_EXISTENT_OPERATOR, "Non existent or implemented operator" },
	};

	auto tuple = default_message.find(id);
	
	if (tuple != default_message.end())
		message = err_prefix + " " + tuple->second;
	
	if (not additional_info.empty())
		info = info_prefix + " " + additional_info;
	
	this->file_name = file_name;
	this->file_path = file_path;
	this->row = row;
	this->column = column;
}


int interpreter_error::get_number_id()
{
	return static_cast<int>(id);
}


interpreter_error* interpreter_error::set_position(string file_name, string file_path, size_t row, size_t column)
{
	this->file_name = file_name;
	this->file_path = file_path;
	this->row = row;
	this->column = column;
	
	return this;
}


void exit_with(interpreter_error* err)
{
	int exit_number = err->get_number_id();
	string pos = "";
	string message = err->message;
	
	// Get the position only if available
	if (not err->file_path.empty())
	{
		pos += err->file_path;

		if (err->row)
		{
			pos += ":" + to_string(err->row);
			
			if (err->column)
			{
				pos += ":" + to_string(err->column);
			}
		}
	}
	
	// Add the position to the message if available
	if (not pos.empty())
		message += " at \"" + pos + "\"";

	// Write error message
	cout << endl << color(message.c_str(), RED) << endl;
	cout << endl << color(err->info.c_str(), RED) << endl;
	
	delete err;

	// BUG: It shows a "random" number, not the intended one
	exit(exit_number);
}
