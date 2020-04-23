#pragma once
#include <iostream>
#include "Lexer.h"
#include "Env.h";

using namespace std;

class Parser
{
private:
	int bracket_balance;
	int operator_brackets_balance;

	streampos tmp_current_file_pos;

	Token* current_token;
	Token* tmp_current_token;
	Lexer* lexer;

	bool parse_expr();
	bool parse_bool_expr();

	bool parse_subprogramm(CheckTokenType type);

	bool parse_call();
	bool parse_param_list();
	bool parse_call_param_list();
	bool parse_var(bool global = false);

	void save_state();
	void load_state();

	//var - когда парсим объ€вление переменных 								
	bool match(Token* token, bool show_error = true);
	bool match(TokenType token_type, bool show_error = true);
	bool stmt();

public:
	void Parse(string path);

	Parser();
	~Parser();
};

