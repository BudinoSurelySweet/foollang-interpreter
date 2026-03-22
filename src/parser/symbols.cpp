#include "symbols.hpp"

#include <type_traits>
#include <unordered_map>
#include <variant>

#include "error_manager.hpp"
#include "token.hpp"


Environment global_symbols = Environment();
Environment static_symbols = Environment();


void Environment::define(const string &name, LangType value)
{
	static auto err = new interpreter_error(error_id::DUPLICATE_VARIABLE);

	err->set_info("Variable with name '" + name + "' already exists.");

	if (symbols.find(name) != symbols.end())
		exit_with(err);

	symbols.insert({name, value});
}


LangType Environment::get(token* variable)
{
	static auto err = new interpreter_error(error_id::NON_EXISTENT_VARIABLE);

	err->set_position(variable);

	bool var_isnt_word = variable->type != token_type::WORD;
	bool var_isnt_symbol = symbols.find(variable->lexeme) == symbols.end();

	if (var_isnt_word or var_isnt_symbol)
		exit_with(err);

	return symbols[variable->lexeme];
}


void Environment::set(const string &name, LangType value)
{
	static auto err = new interpreter_error(error_id::NON_EXISTENT_VARIABLE);

	auto put_value_inside_variable = [this, name, value](auto& new_value, auto& symbol_value)
	{
		using curr_type = decay_t<decltype(symbol_value)>;

		symbol_value = static_cast<curr_type>(new_value);
	};

	bool var_isnt_symbol = symbols.find(name) == symbols.end();

	if (var_isnt_symbol)
		exit_with(err);

	visit(put_value_inside_variable, value, symbols[name]);
}


unordered_map<string, LangType>& Environment::table()
{
	return symbols;
}

