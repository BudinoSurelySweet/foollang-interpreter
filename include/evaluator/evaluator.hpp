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


// TODO: Rinominare i nomi con "symbol tables"
// HACK: Sostituire la variant con i tipi "sicuri", e.g. int16_t, int32_t, ...

inline auto variables_8bit = unordered_map<string, variant<byte, signed char, unsigned char>>();
inline auto variables_16bit = unordered_map<string, variant<int16_t, uint16_t>>();
inline auto variables_32bit = unordered_map<string, variant<int32_t, uint32_t, float>>();
inline auto variables_64bit = unordered_map<string, variant<int64_t, uint64_t, double>>();


void evaluate_operands(vector<token>* tokens, token* op, size_t pos);


#endif
