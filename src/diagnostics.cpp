#include "diagnostics.hpp"


void push_error(string error)
{
	cout << "ERROR: " << error << endl;
	
	cout << "CONSEQUENCE: File interpretation failed." << endl;
	
	exit(1);
}
