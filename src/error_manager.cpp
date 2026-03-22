#include "error_manager.hpp"

#include <iostream>

#include "token.hpp"
#include "color.hpp"

using namespace std;


interpreter_error::interpreter_error(error_id id, string info, string file_name, string file_path, size_t row, size_t column)
{
	auto tuple = DEFAULT_ERROR_MESSAGE.find(id);
	
	if (tuple != DEFAULT_ERROR_MESSAGE.end())
		message = ERROR_PREFIX + " " + tuple->second;
	
	if (not info.empty())
		this->info = INFO_PREFIX + " " + info;
	
	this->file_name = file_name;
	this->file_path = file_path;
	this->row = row;
	this->column = column;
}


interpreter_error::interpreter_error(error_id id, token* fault_target, string info)
{
	auto tuple = DEFAULT_ERROR_MESSAGE.find(id);
	
	if (tuple != DEFAULT_ERROR_MESSAGE.end())
		message = ERROR_PREFIX + " " + tuple->second;
	
	if (not info.empty())
		this->info = INFO_PREFIX + " " + info;

	// FIX: Add an error
	if (fault_target)
		exit(1);

	this->file_name = fault_target->file_name;
	this->file_path = fault_target->file_path;
	this->row = fault_target->row;
	this->column = fault_target->column;
}


int interpreter_error::get_number_id()
{
	return static_cast<int>(id);
}


interpreter_error* interpreter_error::set_position(string& file_name, string& file_path, size_t row, size_t column)
{
	this->file_name = file_name;
	this->file_path = file_path;
	this->row = row;
	this->column = column;
	
	return this;
}


interpreter_error* interpreter_error::set_position(token* fault_target)
{
	this->file_name = fault_target->file_name;
	this->file_path = fault_target->file_path;
	this->row = fault_target->row;
	this->column = fault_target->column;

	return this;
}


interpreter_error* interpreter_error::set_message(const string &message)
{
	this->message = message;

	return this;
}


interpreter_error* interpreter_error::set_info(const string &info)
{
	this->info = info;

	return this;
}


string interpreter_error::get_message()
{
	return message;
}


string interpreter_error::get_info()
{
	return info;
}


void exit_with(interpreter_error* err)
{
	int exit_number = err->get_number_id();
	string pos = "";
	string message = err->get_message();
	
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
	cout << endl << color(err->get_info().c_str(), RED) << endl;
	
	delete err;

	// BUG: It shows a "random" number, not the intended one
	exit(exit_number);
}

