#pragma once
#include <iostream>
#include "Lexer.h"
#include "Env.h"
#include "GlobalHelper.h"
#include "BuildingTree.h"

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
	Token* pred_token;
	Token* tmp_current_token;
	Token* current_struct;

	bool in_block;

	Lexer* lexer;

	Node* parse_expr();
	Node* parse_bool_expr();

	Node* parse_subprogramm(CheckTokenType type, bool global = true);

	Node* parse_call(Token* subprogram_token);
	Node* parse_param_list(vector<Variable>& signature);
	Node* parse_call_param_list(vector<Variable>& signature);
	Node* parse_var(bool global = false, bool in_struct = false);

	Node* parse_struct();

	void save_state();
	void load_state();

	bool is_user_datatype(Token*& token);

	DataTypes define_data_type(Token* token);
							
	bool match(Token* token, bool show_error = true);
	bool match(TokenType token_type, bool show_error = true);
	Node* stmt();

public:
	void Parse(string path, Node*& tree);

	Parser();
	~Parser();
};

