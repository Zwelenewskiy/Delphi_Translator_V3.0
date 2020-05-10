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
#include "Parser.h"

using namespace std;

string const PATH = "input.pas";

int main()
{
	Node* builded_tree = nullptr;
	Parser* parser = new Parser();
	parser->Parse(PATH, builded_tree);

	system("pause");

	delete parser;
	return 0;
}