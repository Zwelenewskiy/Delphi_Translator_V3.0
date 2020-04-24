#pragma once
#include <iostream>
#include "Lexer.h"
#include "GlobalHelper.h"

using namespace std;

enum EnvType {
	Local,
	Global
};

class Env
{
private:
	vector<Token*> table;
	bool check_signature(Token* token_1, Token* token_2);
public:
	Env();
	~Env();

	void put(Token*  token);
	bool get(Token*& token);
};

