#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <assert.h>

#include "lexer.hpp"

using namespace std;


class ast_base
{
protected:
	token_type type;

public:
	bool is_primitive;
	
	token_type get_type() const;
};


template <typename T>
class ast_primitive : public ast_base
{
public:
	T value;

	ast_primitive(T new_value)
	{
		value = new_value;
		is_primitive = true;
	}
	
	T evaluate() const
	{
		return value;
	}
};


class ast_derivated : public ast_base
{
public:
	ast_base* left;
	ast_base* right;
	ast_base* associated_primitive;
	
	ast_derivated(ast_base* new_left = nullptr, ast_base* new_right = nullptr);
	
	virtual void evaluate();
	
	~ast_derivated();
};


class ast_expression : public ast_base
{
public:
	ast_base* down;
	
	ast_expression(linked_list<token*>* linear_expression);
	
	int32_t evaluate();
};


class ast_i32 : public ast_primitive<int32_t>
{
public:
	ast_i32(int32_t new_value);
};


class ast_operator_plus : public ast_derivated
{
public:
	ast_operator_plus();
	
	void evaluate() override;
};


linked_list<ast_expression*>* parser(linked_list<token*>* head_token);


#endif
