#pragma once

#include <charconv>
#include <string>
#include <system_error>

using namespace std;


template <typename T>
T parse_integer(const string& str)
{
	T result = {};

	auto [ptr, ec] = from_chars(str.data(), str.data() + str.size(), result);

	if (ec == errc::invalid_argument) {
		throw std::runtime_error("Non è un numero: " + str);
	} else if (ec == std::errc::result_out_of_range) {
		throw std::runtime_error("Numero troppo grande per il tipo richiesto: " + str);
	}

	return result;
};


template <typename T>
bool can_be_parsed_as_integer(const string& str)
{
	T result = {};

	auto [ptr, ec] = from_chars(str.data(), str.data() + str.size(), result);

	if (ec == errc::invalid_argument)
		return false;
	else if (ec == std::errc::result_out_of_range)
		return false;

	return true;
}

