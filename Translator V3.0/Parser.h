#pragma once
#include <iostream>
#include "Lexer.h"

using namespace std;

class Parser
{
private:
	int bracket_balance;

	Token* current_token;
	Lexer* lexer;
	bool parse_ariphmethical_expr();
	bool parse_bool_expr();
	bool parse_var();
	bool match(Token* token, bool show_error = true);
	bool match(TokenType token_type, bool show_error = true);
	bool stmt();

public:
	void Parse(string path);

	Parser();
	~Parser();
};

