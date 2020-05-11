#include "BuildingTree.h"

void BuildingTree::infix_to_postfix(Token* token, TreeType type, bool end)
{
	if (!end) {
		//определяем приоритет операций
		if (type == BooleanExpr)// для булевских выражений
		{
			prec["="] = 5;
			prec[">"] = 5;
			prec["<"] = 5;
			prec["<="] = 5;
			prec["=>"] = 5;
			prec["and"] = 3;
			prec["or"] = 2;
			prec["not"] = 1;
		}
		else //для арифметических
		{
			prec["*"] = 3;
			prec["/"] = 3;
			prec["+"] = 2;
			prec["-"] = 2;
		}

		prec["("] = 0;
		if ((Match_Reg(token->value, DIGIT) || Match_Reg(token->value, IDENTIFICATOR))
			&& (token->type != AriphmethicalOperator) && (token->type != LogicalOperator))
		{
			postfix.push_back(token);
		}
		else if (token->value == "(")
		{
			operation_stack.push(token);
		}
		else if (token->value == ")")
		{
			Token* top_token = operation_stack.top();
			operation_stack.pop();
			while (top_token->value != "(")
			{
				postfix.push_back(top_token);
				top_token = operation_stack.top();
				operation_stack.pop();
			}
		}
		else
		{
			while ((!operation_stack.empty()) && (prec[operation_stack.top()->value] >= prec[token->value]))
			{
				postfix.push_back(operation_stack.top());
				operation_stack.pop();
			}

			operation_stack.push(token);
		}
	}
	else {
		while (!operation_stack.empty())
		{
			postfix.push_back(operation_stack.top());
			operation_stack.pop();
		}
	}
	
}

Node* BuildingTree::build_tree()//функция для построения дерева
{
	static stack<Node*> tmp_stack;

	while (!tmp_stack.empty())
		tmp_stack.pop();

	for (int i = 0; i < postfix.size(); i++)
	{
		Node* tmp = new Node();
		tmp->data.push_back(postfix[i]);

		if ((Match_Reg(postfix[i]->value, DIGIT) || Match_Reg(postfix[i]->value, IDENTIFICATOR))
			&& (postfix[i]->type != AriphmethicalOperator) && (postfix[i]->type != LogicalOperator))
		{
			if (!tmp_stack.empty())
				tmp->next = tmp_stack.top();
			tmp_stack.push(tmp);
		}
		else
		{
			tmp->right = tmp_stack.top();

			if (postfix[i]->value != "not") {
				tmp->left = tmp_stack.top()->next;
				tmp->next = tmp_stack.top()->next->next;
			}

			tmp_stack.push(tmp);
		}
	}

	return tmp_stack.top();
}