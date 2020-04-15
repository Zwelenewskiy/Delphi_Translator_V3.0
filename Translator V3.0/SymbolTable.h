#pragma once
#include <iostream>
#include "Lexer.h"
#include "GlobalHelper.h"

using namespace std;

vector<Token*> symbol_table;

bool exist_in_table(Token*& token) {
	for (int i = 0; i < symbol_table.size(); i++) {
		if (to_lower(symbol_table[i]->value) == to_lower(token->value)) {
			token = symbol_table[i];
			return true;
		}
	}

	return false;
}

void add_in_table(Token*& token, CheckTokenType type) {
	symbol_table.push_back(token);

	token->id = symbol_table.size() - 1;
	token->check_type = type;
}

/*bool exist_in_table(Token*& token, CheckTokenType type = Undefined, bool add = true) {
	for (int i = 0; i < symbol_table.size(); i++) {
		if (symbol_table[i]->value == token->value) {
			if (type != Undefined) {
				if(symbol_table[i]->check_type == type)
					return true;
			}
			else
				return true;
		}
			
	}

	if (add) {
		symbol_table.push_back(token);

		token->id = symbol_table.size() - 1;
		token->check_type = type;
	}	

	return false;
}*/