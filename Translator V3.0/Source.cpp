#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <map>
#include <queue>
#include <regex>
#include "GlobalHelper.h"
#include "Lexer.h"

using namespace std;

string const PATH = "input.pas";

int main()
{
	Lexer* lexer = new Lexer(PATH);
	Token* token;

	while (true) {
		token = lexer->GetToken();
		if (token == nullptr)
			break;

		cout << token->value << endl;
	}

	system("pause");

	delete lexer;
	return 0;
}