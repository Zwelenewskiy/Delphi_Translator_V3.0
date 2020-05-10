#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <map>
#include <queue>
#include <regex>
#include "Lexer.h"
using namespace std;

#define IDENTIFICATOR "([A-Z]|[a-z])([A-Z]|[a-z]|[0-9])*"
#define INTEGER "^-?[0-9]+$"
#define DIGIT "^-?[0-9]*[.,]?[0-9]+$"
#define CHAR R"(^'\S{1}')"
#define STRING R"('([A-Z]|[a-z]|[0-9]|\s|\W)*')"

bool Match_Reg(string input, string pattern);
string to_lower(string s);
void ShowError(string message);