#include "Env.h"

Env::Env() {}

Env::~Env(){}

//true, ���� ��������� ����������������
//false, ���� ��������� ������
bool Env::check_signature(Token * token_1, Token * token_2)
{
	if (token_1->signature.size() != token_2->signature.size())
		return false;
	else
	{
		for (size_t i = 0; i < token_1->signature.size(); i++)
		{
			if (token_1->signature[i].data_type != token_2->signature[i].data_type)
				return false;
		}

		return true;
	}
}

//false, ���� �� ������� �� ����� ���������� ������������
//true, ���� ������� ���������� ������������
bool Env::check_signature(Token* token, vector<Variable> signature, Env * env)
{
	vector<Token*> overloaded_subprograms;
	vector<Token*> current_table;

	if (env != nullptr)
		current_table = env->table;
	else
		current_table = table;

	for (int i = 0; i < current_table.size(); i++) {
		if (current_table[i]->value == token->value) {
			if ((current_table[i]->check_type == Procedure)
				|| (current_table[i]->check_type == Function))
			{
				overloaded_subprograms.push_back(current_table[i]);
			}
		}
	}

	for (size_t i = 0; i < overloaded_subprograms.size(); i++)
	{		
		Token* tmp = new Token();
		tmp->signature = signature;

		if (!check_signature(overloaded_subprograms[i], tmp))
			return false;
	}	

	return true;
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

void Env::show()
{
	for (Token* token : table) {
		if(token->parent != nullptr)
			cout << token->value << ": " << token->data_type << ": " << token->modifier << ": " << token->parent->value << endl;
		else
			cout << token->value << ": " << token->data_type << ": " << token->modifier << ": " << endl;
	}
}
