#pragma once
#include <iostream>
#include "Lexer.h"

using namespace std;

class Parser
{
private:
	Token* current_token;
	Lexer* lexer;
	bool parse_ariphmethical_expr();
	bool match(Token* token, bool show_error = true);
	bool match(TokenType token_type, bool show_error = true);
	bool stmt();

public:
	void Parse(string path);

	Parser();
	~Parser();
};

