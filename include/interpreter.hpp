#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <string>

using namespace std;


static void update_position(char* character, size_t* row, size_t* column);


void interpret(string* source_code, string file_name, string file_path);


#endif
