#pragma once
#include "Lexer.h"

enum TreeType {
	AriphmethicalExpr,
	Bool
};

class BuildingTree {
private:
	stack<string> operation_stack;
	stack <Node*> tmp_stack;
	map<string, int> prec;//����������� ��������
	vector<Token*> postfixList;

	vector<Token*> infix_to_postfix(Token* token, TreeType type);
	Node*  build_tree(Token* token, bool end);//������� ��� ���������� ������;
public:
	Node* create_ast_tree(Token* token, TreeType type, bool end = false);
};