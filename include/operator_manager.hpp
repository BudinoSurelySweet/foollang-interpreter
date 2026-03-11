#ifndef OPERATOR_MANAGER_HPP
#define OPERATOR_MANAGER_HPP

#include <vector>

#include "token.hpp"


class operator_manager
{
	vector<vector<size_t>> levels_of_precedence;
	int target_precedence_level;


public:
	operator_manager();
	void add(token_type type, size_t pos);
	vector<size_t> get();
	void reset();
};

#endif
