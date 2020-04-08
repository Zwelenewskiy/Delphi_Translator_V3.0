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

public:
	void Parse(string path);
	bool stmt();
	bool match(Token* token);
	bool match(TokenType token_type);

	Parser();
	~Parser();
};

