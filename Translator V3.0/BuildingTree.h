#pragma once
#include "Lexer.h"

enum TreeType {
	AriphmethicalExpr,
	BooleanExpr
};

class BuildingTree {
private:
	stack<Token*> operation_stack;
	stack <Node*> tmp_stack;
	map<string, int> prec;//старшинство операций
	//vector<Token*> postfix;
	vector<Node*> postfix;

	//vector<Token*> sequence_array;
	vector<Node*> sequence_array;
	int sequence_array_position;

public:
	void infix_to_postfix(Token* token, TreeType type, bool end = false, bool start_sequence = false, bool add_in_sequence_array = false, Node* node = nullptr);
	Node* build_tree();//функция для построения дерева;
};