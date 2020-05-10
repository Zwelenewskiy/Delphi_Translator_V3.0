#include "BuildingTree.h"

vector<Token*> BuildingTree::infix_to_postfix(Token* token, TreeType type)
{
	//определяем приоритет операций
	if (type == Bool)// для булевских выражений
	{
		prec["="] = 5;
		prec[">"] = 5;
		prec["<"] = 5;
		prec["<="] = 5;
		prec["=>"] = 5;
		prec["<>"] = 5;
		prec["!"] = 4;
		prec["&&"] = 3;
		prec["||"] = 2;
	}
	else //для арифметических
	{
		prec["*"] = 3;
		prec["/"] = 3;
		prec["+"] = 2;
		prec["-"] = 2;
	}

	prec["("] = 1;

	//Token* token = lexer->GetToken();
	//while (token->value != ";")
	//{
	//определение типа токена
	if (Match_Reg(token->value, DIGIT) || Match_Reg(token->value, IDENTIFICATOR))
	{
		postfixList.push_back(token);
	}
	else if (token->value == "(")
	{
		operation_stack.push(token->value);
	}
	else if (token->value == ")")
	{
		string top_token = operation_stack.top();
		operation_stack.pop();
		while (top_token != "(")
		{
			postfixList.push_back(new Token(top_token));
			top_token = operation_stack.top();
			operation_stack.pop();
		}
	}
	else
	{
		while ((!operation_stack.empty()) && (prec[operation_stack.top()] >= prec[token->value]))
		{
			postfixList.push_back(new Token(operation_stack.top()));
			operation_stack.pop();
		}

		operation_stack.push(token->value);
	}
	//}

	while (!operation_stack.empty())
	{
		postfixList.push_back(new Token(operation_stack.top()));
		operation_stack.pop();
	}

	return postfixList;
}

Node*  BuildingTree::build_tree(Token* token, bool end)//функция для построения дерева
{
	Node* tmp = new Node();
	tmp->data.push_back(token);

	if (Match_Reg(token->value, DIGIT) || Match_Reg(token->value, IDENTIFICATOR))
	{
		if (!tmp_stack.empty())
			tmp->next = tmp_stack.top();
		tmp_stack.push(tmp);
	}
	else
	{
		tmp->right = tmp_stack.top();

		if (token->value != "not") {
			tmp->left = tmp_stack.top()->next;
			tmp->next = tmp_stack.top()->next->next;
		}

		tmp_stack.push(tmp);
	}

	if (end)
		return tmp_stack.top();
	else
		return nullptr;
}

Node* BuildingTree::create_ast_tree(Token* token, TreeType type, bool end) {
	infix_to_postfix(token, type);

	if (end)
		return build_tree(token, true);
	else {
		build_tree(token, false);
		return nullptr;
	}
}