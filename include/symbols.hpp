#pragma once

#include <cstdint>
#include <unordered_map>
#include <variant>
#include <string>

#include "token.hpp"
#include "error_manager.hpp"


using namespace std;


using LangType = variant<
	char8_t,
	char16_t,
	char32_t,

	int8_t,
	int16_t,
	int32_t,
	int64_t,

	uint8_t,
	uint16_t,
	uint32_t,
	uint64_t,

	float,
	double
>;


class Environment
{
private:
	unordered_map<string, LangType> symbols;

public:
	void define(const string &name, LangType value);
	LangType get(token* variable);
	void set(const string &name, LangType value);
	unordered_map<string, LangType>& table();
};

extern Environment global_symbols;
extern Environment static_symbols;

