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
public:
	Env(EnvType t);
	Env();
	~Env();

	vector<Token*> table;
	void put(Token*  token);
	bool get(Token*& token);
};

