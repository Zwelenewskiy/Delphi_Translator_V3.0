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
	Parser* parser = new Parser();
	Node* builded_tree = new Node();

	parser->Parse(PATH, builded_tree);

	//show_mixed(builded_tree, 0);

	system("pause");

	delete parser;
	return 0;
}