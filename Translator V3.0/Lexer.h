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

enum TokenType
{
	Identificator,
	KeyWord,
	LogicalOperator,
	AriphmethicalOperator,
	Separator,
	Literal,
	Operator,
	ErrorToken
};

class Token
{
public:
	TokenType type;
	string value;
};

class Lexer
{
private:
	string PATH;
	streampos current_file_pos;

	Token* DefineTokenType(string lexem);

public:
	Lexer(string path);
	~Lexer();

	Token* GetToken();
};

