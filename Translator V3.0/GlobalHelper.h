#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <map>
#include <queue>
#include <regex>
using namespace std;

#define IDENTIFICATOR "([A-Z]|[a-z])([A-Z]|[a-z]|[0-9])*"
#define INTEGER "^-?[0-9]+$"
#define DIGIT "^-?[0-9]*[.,]?[0-9]+$"
#define CHAR R"(^'\S{1}')"
#define STRING R"('([A-Z]|[a-z]|[0-9]|\s|\W)*')"

struct Node//структура, описывающая узел AST-дерева
{
	string name;
	string data;

	Node* left;
	Node* right;
	Node* next;

	Node* condition;//для поддерева булевого выражения

	Node()
	{
		name = "";
		data = "";
		left = NULL;
		right = NULL;
		condition = NULL;
	}
};

vector<string> InfixToPostfix(string infix, char type = 'a');
bool Match_Reg(string input, string pattern);
string to_lower(string s);

Node* buildATree(vector <string> postfix);
void showMixed(Node* root, int depth);
string get_token();