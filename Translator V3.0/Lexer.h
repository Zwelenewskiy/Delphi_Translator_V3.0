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

enum DataTypes {
	Integer,
	Double,
	Char,
	String,
	Boolean
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

struct Variable {
	string name;
	DataTypes data_type;

	Variable(string n, DataTypes t) {
		name = n;
		data_type = t;
	}
};

class Token
{
public:
	TokenType type;
	string value;
	CheckTokenType check_type;
	vector<Variable> signature;
	DataTypes data_type;

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

