#include "BuildingTree.h"

void BuildingTree::infix_to_postfix(Token* token, TreeType type, bool end, bool start_sequence, bool add_in_sequence_array)
{
	if(token)
		token = new Token(token->value);

	if (start_sequence) {
		sequence_array.push_back(nullptr);

		sequence_array_position = sequence_array.size() - 1;
		token->sequence_position = sequence_array_position;
	}
	else if(add_in_sequence_array) {
		Token* tmp = sequence_array[sequence_array_position];

		if (tmp) { 
			while (tmp->next)
				tmp = tmp->next;

			tmp->next = token;
		}
		else {
			sequence_array[sequence_array_position] = token;
		}

		return;
	}

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
			//postfix.push_back(token);
			postfix.push_back(new Node(token));
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
				//postfix.push_back(top_token);
				postfix.push_back(new Node(top_token));
				top_token = operation_stack.top();
				operation_stack.pop();
			}
		}
		else
		{
			while ((!operation_stack.empty()) && (prec[operation_stack.top()->value] >= prec[token->value]))
			{
				//postfix.push_back(operation_stack.top());
				postfix.push_back(new Node(operation_stack.top()));
				operation_stack.pop();
			}

			operation_stack.push(token);
		}
	}
	else {
		while (!operation_stack.empty())
		{
			//postfix.push_back(operation_stack.top());
			postfix.push_back(new Node(operation_stack.top()));
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
		//tmp->data = postfix[i];
		tmp = postfix[i];

		if ((Match_Reg(postfix[i]->data->value, DIGIT) || Match_Reg(postfix[i]->data->value, IDENTIFICATOR))
			&& (postfix[i]->data->type != AriphmethicalOperator) && (postfix[i]->data->type != LogicalOperator))
		{
			if (postfix[i]->data->sequence_position != -1) {
				Token* tmp_token = sequence_array[postfix[i]->data->sequence_position];
				while (tmp_token) {
					Node* t = new Node();
					t->data = tmp_token;

					Node* tmp_node = tmp->sequence;
					if (tmp_node) {
						while (tmp_node->next)
							tmp_node = tmp_node->next;

						tmp_node->next = t;
					}
					else {
						tmp->sequence = t;
					}

					tmp_token = tmp_token->next;
				}
			}			

			if (!tmp_stack.empty())
				tmp->next = tmp_stack.top();
			tmp_stack.push(tmp);
		}
		else
		{
			tmp->right = tmp_stack.top();

			if (postfix[i]->data->value != "not") {
				tmp->left = tmp_stack.top()->next;
				tmp->next = tmp_stack.top()->next->next;
			}

			tmp_stack.push(tmp);
		}
	}

	return tmp_stack.top();
}