#include "GlobalHelper.h"

using namespace std; 

string to_lower(string s) {
	transform(s.begin(), s.end(), s.begin(), tolower);
	return s;
}

void ShowError(string message)
{
	system("color 4");

	cout << endl << message << endl;
}

bool Match_Reg(string input, string pattern)//проверка на соответствие шаблону
{
	regex control(pattern);
	smatch match;

	return regex_match(input, match, control);
}