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

enum  CheckTokenType
{
	Var,
	Function,
	Procedure,
	Undefined
};

enum TokenType
{
	Identificator,//0
	KeyWord,//1
	LogicalOperator,//2
	AriphmethicalOperator,//3
	Separator,//4
	Literal,//5
	Operator,//6
	ErrorToken,//7
	TypeData//8
};

class Token
{
public:
	TokenType type;
	string value;
	CheckTokenType check_type;
	int id;

	Token();
	Token(string val);
};

class Lexer
{
private:
	string PATH;
	Token* DefineTokenType(string lexem);

public:
	streampos current_file_pos;

	Lexer(string path);
	~Lexer();

	Token* GetToken();
};

