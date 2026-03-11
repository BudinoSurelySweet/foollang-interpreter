#include "operator_manager.hpp"


operator_manager::operator_manager()
{
	// TODO Remove this magic number
	levels_of_precedence.resize(14);
}


void operator_manager::add(token_type type, size_t pos)
{
	target_precedence_level = operator_precedence_to_int(type);
	
	if (target_precedence_level < 1 or (size_t)target_precedence_level > levels_of_precedence.size())
		return;

	// Add the element to the corrispondent level
	auto& lvl = levels_of_precedence.at(target_precedence_level - 1);
	lvl.push_back(pos);
}


vector<size_t> operator_manager::get()
{
	vector<size_t> result;

	for (auto& row_vector : levels_of_precedence)
		for (auto tok : row_vector)
			result.push_back(tok);
	
	return result;
}


void operator_manager::reset()
{
	for (auto& lvl : levels_of_precedence)
		lvl.clear();
}
