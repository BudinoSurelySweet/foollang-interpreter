#include "interpreter.hpp"
#include "token.hpp"


static token* create_token(string* source_code, size_t pos)
{
	static token_type context_type = token_type::NONE;
	static size_t context_size = 1;
	static string* context;
	
	char character = (*source_code)[pos];
	char_type character_type = get_char_type(character);
	
	if (not (int) context_type)
	{
		context = &source_code[pos];
		
		// TODO Continuare da qui
	}
}


void interpret(string* source_code)
{
	int i = 0;
	string::iterator target = (*source_code).begin();

	for (; target != (*source_code).end(); ++target, ++i)
	{
		create_token(source_code, i);
	}
}
