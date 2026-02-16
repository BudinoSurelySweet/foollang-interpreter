#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>

#include "linked_list.hpp"
#include "token.hpp"

using namespace std;


llist<token*>* lexer(string* source_code);


#endif
