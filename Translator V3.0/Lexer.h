#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "GlobalHelper.h"

using namespace std;

enum  CheckTokenType
{
	Var,
	Function,
	Procedure,
	Class,
	Record,
	UndefinedCheckTokenType
};

enum DataTypes {
	Integer,
	Double,
	Char,
	String,
	Boolean,
	UserDataType,
	UndefinedDataType
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

enum Modifier {
	Public,
	Private,
	Protected
};

struct Variable {
	string name;
	DataTypes data_type;

	Variable(string n, DataTypes t) {
		name = n;
		data_type = t;
		
	}
};

class Env;

class Token
{
public:
	TokenType type;
	string value;
	CheckTokenType check_type;
	vector<Variable> signature;
	DataTypes data_type;
	Env* members;
	Modifier modifier;
	Token* parent;

	Token* next;
	int  sequence_position;

	int line;

	Token();
	Token(string val);
};

struct Node//узел AST-дерева
{
public:
	Token* data;

	Node* left;
	Node* right;
	Node* next;//для элемента block
	Node* sequence;//для последовательности обращения к полям структур
	string name;

	Node* condition;//для булевого выражения

	Node()
	{
		left = nullptr;
		right = nullptr;
		condition = nullptr;
		next = nullptr;
		sequence = nullptr;
		name = "";
	}

	Node(Token* token) {
		data = token; 
		left = nullptr;
		right = nullptr;
		condition = nullptr;
		next = nullptr;
		sequence = nullptr;
		name = "";
	}
};

class Lexer
{
private:
	string PATH;
	Token* DefineTokenType(string lexem);

public:
	streampos current_file_pos;
	int current_line;

	Lexer(string path);
	~Lexer();

	Token* GetToken();
};