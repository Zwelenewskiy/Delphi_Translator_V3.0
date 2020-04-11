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
	parser->Parse(PATH);

	system("pause");

	delete parser;
	return 0;
}