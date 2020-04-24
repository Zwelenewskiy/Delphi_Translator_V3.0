#include "Env.h"

Env::Env() {}

Env::~Env(){}

//true, если сигнатуры разные
//false, если сигнатуры одинаковые
bool Env::check_signature(Token * token_1, Token * token_2)
{
	if (token_1->signature.size() != token_2->signature.size())
		return true;
	else
	{
		for (size_t i = 0; i < token_1->signature.size(); i++)
		{
			if ((token_1->signature[i].name == token_2->signature[i].name)
				&& (token_1->signature[i].data_type == token_2->signature[i].data_type))
				return false;
		}

		return true;
	}
}

void Env::put(Token* token)
{
	table.push_back(token);
}

bool Env::get(Token*& token, bool check_sign)
{
	for (int i = 0; i < table.size(); i++) {
		if (table[i]->value == token->value) {
			if (check_sign) {
				if ((table[i]->check_type == Procedure)
					|| (table[i]->check_type == Function))
				{
					if (!check_signature(token, table[i])) {
						token = table[i];
						return true;
					}
				}
			}			
			else {
				token = table[i];
				return true;
			}			
		}
	}

	return false;
}
