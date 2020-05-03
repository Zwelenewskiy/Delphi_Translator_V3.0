#pragma once
#include <iostream>
#include "Lexer.h"
#include "Env.h"
#include "GlobalHelper.h"

using namespace std;

class Parser
{
private:
	int bracket_balance;
	int operator_brackets_balance;

	Env* global_env = new Env();
	Env* current_env = new Env();

	vector<Token*> user_datatypes;

	Modifier current_modifier;

	streampos tmp_current_file_pos;

	Token* current_token;
	Token* tmp_current_token;
	Lexer* lexer;

	bool parse_expr();
	bool parse_bool_expr();

	bool parse_subprogramm(CheckTokenType type, bool global = true);

	bool parse_call(Token* subprogram_token);
	bool parse_param_list(vector<Variable>& signature);
	bool parse_call_param_list(vector<Variable>& signature);
	bool parse_var(bool global = false, bool in_struct = false);

	bool parse_struct();

	void save_state();
	void load_state();

	bool is_user_datatype(Token*& token);

	DataTypes define_data_type(Token* token);
							
	bool match(Token* token, bool show_error = true);
	bool match(TokenType token_type, bool show_error = true);
	bool stmt();

public:
	void Parse(string path);

	Parser();
	~Parser();
};

