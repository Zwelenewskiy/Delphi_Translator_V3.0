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
	Token* tmp_current_token;
	Token* current_struct;

	Lexer* lexer;

	Node* parse_expr(Node* tree);
	Node* parse_bool_expr(Node* tree);

	Node* parse_subprogramm(CheckTokenType type, Node* tree, bool global = true);

	Node* parse_call(Token* subprogram_token, Node* tree);
	Node* parse_param_list(vector<Variable>& signature, Node* tree);
	Node* parse_call_param_list(vector<Variable>& signature, Node* tree);
	Node* parse_var(Node* tree, bool global = false, bool in_struct = false);

	Node* parse_struct(Node* tree);

	void save_state();
	void load_state();

	bool is_user_datatype(Token*& token);

	DataTypes define_data_type(Token* token);
							
	bool match(Token* token, bool show_error = true);
	bool match(TokenType token_type, bool show_error = true);
	Node* stmt(Node* tree);

public:
	void Parse(string path, Node* tree);

	Parser();
	~Parser();
};

