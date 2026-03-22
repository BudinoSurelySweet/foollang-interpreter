#pragma once

#include <vector>

#include "token.hpp"
#include "error_manager.hpp"

using namespace std;


struct operands_group
{
	token* leftmost;
	token* left;
	token* right;
	token* rightmost;
	
	operands_group(token* leftmost, token* left, token* right, token* rightmost);
};


void evaluate_operands(vector<token>* tokens, token* op, size_t pos);

