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
	vector<Token*> postfix;

public:
	void infix_to_postfix(Token* token, TreeType type, bool end = false);
	Node* build_tree();//функция для построения дерева;
};