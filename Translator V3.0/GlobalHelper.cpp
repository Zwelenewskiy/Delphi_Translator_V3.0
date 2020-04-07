#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <map>
#include <queue>
#include <regex>
#include "GlobalHelper.h"

using namespace std;

string to_lower(string s) {
	transform(s.begin(), s.end(), s.begin(), tolower);
	return s;
}

bool Match_Reg(string input, string pattern)//проверка на соответствие шаблону
{
	regex control(pattern);
	smatch match;

	return regex_match(input, match, control);
}

vector<string> InfixToPostfix(string infix, char type)
{
	stack<string> operation_stack;
	map<string, int> prec;//старшинство операций
	vector<string> postfixList;
	//определяем приоритет операций
	if (type == 'b')// для булевских выражений
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

	stringstream st(infix);
	string token;

	while (st >> token)//получение текущего токена из строки (токены разделяются пробелами)
	{
		//определение типа токена
		if (Match_Reg(token, DIGIT) || Match_Reg(token, IDENTIFICATOR))
		{
			postfixList.push_back(token);
		}
		else if (token == "(")
		{
			operation_stack.push(token);
		}
		else if (token == ")")
		{
			string top_token = operation_stack.top();
			operation_stack.pop();
			while (top_token != "(")
			{
				postfixList.push_back(top_token);
				top_token = operation_stack.top();
				operation_stack.pop();
			}
		}
		else
		{
			while ((!operation_stack.empty()) && (prec[operation_stack.top()] >= prec[token]))
			{
				postfixList.push_back(operation_stack.top());
				operation_stack.pop();
			}

			operation_stack.push(token);
		}
	}

	while (!operation_stack.empty())
	{
		postfixList.push_back(operation_stack.top());
		operation_stack.pop();
	}

	return postfixList;
}