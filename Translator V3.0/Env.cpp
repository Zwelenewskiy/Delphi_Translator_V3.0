#include "Env.h"

Env::Env(EnvType t) {}

Env::Env() { }

Env::~Env(){}

void Env::put(Token* token)
{
	table.push_back(token);
}

bool Env::get(Token*& token)
{
	for (int i = 0; i < table.size(); i++) {
		if (table[i]->value == token->value) {
			token = table[i];
			return true;
		}
	}

	return false;
}
