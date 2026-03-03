#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "interpreter.hpp"
#include "color.hpp"

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
		cerr << color("[Error] No file to interpret", RED) << endl;
		
		return 1;
	}
	
	string source_code = read_file(argv[1]);
	
	interpret(&source_code);

	return 0;
}
