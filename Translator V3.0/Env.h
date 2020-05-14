#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Lexer.h"

using namespace std;

enum EnvType {
	Local,
	Global
};

class Env
{
private:
	bool check_signature(Token* token_1, Token* token_2);
public:
	Env();
	~Env();

	vector<Token*> table;

	void put(Token*  token);
	bool check_overloads(Token* token, vector<Variable> signature, Env * env = nullptr);
	bool get(Token*& token, bool check_sign = true);
	void show();
};

