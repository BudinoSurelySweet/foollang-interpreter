#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <cstdint>
#include <vector>
#include <expected>
#include <unordered_map>
#include <variant>
#include <functional>
#include <memory>	

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


// HACK: Sostituire la variant con i tipi "sicuri", e.g. int16_t, int32_t, ...
inline auto variables_8bit = unordered_map<string, variant<char, unsigned char>>();
inline auto variables_16bit = unordered_map<string, variant<short int, unsigned short int>>();
inline auto variables_32bit = unordered_map<string, variant<int, unsigned int, float>>();
inline auto variables_64bit = unordered_map<string, variant<long long int, unsigned long long int, double>>();


void evaluate_operands(vector<token>* tokens, token* op, size_t pos);


#endif
