#include "Lexer.h"
#include "GlobalHelper.h"

Lexer::Lexer(string path)
{
	PATH = path;
	current_file_pos = 0;
}

Lexer::~Lexer(){}

Token* Lexer::GetToken()
{
	Token* tmp = new Token();
	string lexem;
	bool space_flag = false;
	bool number_reading = false;

	bool string_reading = false;
	bool string_reading_start = false;

	bool file_reading = false;

	ifstream file(PATH); 
	char sim;	

	if(current_file_pos != 0)
		file.seekg(current_file_pos);

	while (file.get(sim)) {
		file_reading = true;

		if (sim == '\'') {
			if (!string_reading_start) {
				string_reading = true;
				string_reading_start = true;
			}
			else
				string_reading_start = false;
		}

		if (string_reading) {
			lexem += sim;

			if ((sim == '\'') && string_reading && !string_reading_start) {
				streampos old_pos = file.tellg();

				file.get(sim);
				if (!isalnum(sim)) {
					string_reading = false;

					tmp = DefineTokenType(lexem);
					current_file_pos = old_pos;
					break;
				}
				else {
					file.seekg(old_pos);
				}
			}

			continue;
		}

		if (isdigit(sim)) {
			number_reading = true;
		}
		else if (sim != '.') {
			number_reading = false;
		}

		if (((sim == ' ') || (sim == '\n') || (sim == '\t')) && !space_flag) {
			space_flag = true;
			if (lexem != "") {
				tmp = DefineTokenType(lexem);
				break;
			}

			lexem = "";
			continue;
		}
		else if (((sim == ' ') || (sim == '\n') || (sim == '\t')) && space_flag) {
			continue;
		}

		if (!isalnum(sim))
		{
			if (sim == ':')
			{
				streampos old_pos = file.tellg();

				file.get(sim);
				if (sim == '=') {
					if (lexem != "") {
						tmp = DefineTokenType(lexem);
						break;
					}
					else {
						tmp = DefineTokenType(":=");

						current_file_pos = file.tellg();
						break;
					}
				}
				else {
					file.seekg(old_pos);
				}
			}
			else if((sim == '>') || (sim == '<')){
				streampos old_pos = file.tellg();

				char  old_sim = sim;
				file.get(sim);
				if (sim == '=') {
					if (lexem != "") {
						tmp = DefineTokenType(lexem);
						break;
					}
					else {
						lexem = "";
						lexem += old_sim;
						lexem += sim;
						tmp = DefineTokenType(lexem);

						current_file_pos = file.tellg();
						break;
					}
				}
				else {
					file.seekg(old_pos);
				}
			}
			else
			{
				if (lexem != "") {
					if ((sim == '.') && number_reading) {
						lexem += sim;
						number_reading = false;
						continue;
					}
					else if ((sim == '.') && !number_reading) {
						tmp = DefineTokenType(lexem);
						break;
					}
					else {
						tmp = DefineTokenType(lexem);
						break;
					}
				}				
				else {
					lexem += sim;
					tmp = DefineTokenType(lexem);

					current_file_pos = file.tellg();
					break;
				}
			}

			lexem = "";
		}
		else {
			lexem += sim;

			streampos old_pos = file.tellg();

			if (!file.get(sim)) {
				tmp = DefineTokenType(lexem);
				current_file_pos = file.tellg();
				break;
			}
			else {
				file.seekg(old_pos);
			}

			current_file_pos = file.tellg();
		}

		space_flag = false;
	}

	if (!file_reading) {
		return nullptr;
	}

	return tmp;
}

Token* Lexer::DefineTokenType(string lexem)
{
	Token* token = new Token();

	//cout << lexem;

	if ((lexem == "=") || (lexem == "<") || (lexem == ">") ||
		(lexem == ">=") || (lexem == "<=") || (to_lower(lexem) == "not") || (lexem == "<>")
		|| (to_lower(lexem) == "or") || (to_lower(lexem) == "and")) {
		token->type = LogicalOperator;

		//cout << " LogicalOperator" << endl;
	}
	else if (lexem == ":=") {
		token->type = Operator;

		//cout << " Operator" << endl;
	}
	else if ((to_lower(lexem) == "begin") || (to_lower(lexem) == "end") || (to_lower(lexem) == "if") ||
		(to_lower(lexem) == "then") || (to_lower(lexem) == "else") || (to_lower(lexem) == "for") ||
		(to_lower(lexem) == "while") || (to_lower(lexem) == "to") || (to_lower(lexem) == "downto")
		|| (to_lower(lexem) == "object") || (to_lower(lexem) == "function") || (to_lower(lexem) == "procedure")
		|| (to_lower(lexem) == "nil") || (to_lower(lexem) == "program") || (to_lower(lexem) == "record")
		|| (to_lower(lexem) == "repeat") || (to_lower(lexem) == "var") || (to_lower(lexem) == "integer")
		|| (to_lower(lexem) == "string") || (to_lower(lexem) == "boolean")) {
		token->type = KeyWord;

		//cout << " KeyWord" << endl;
	}
	else if (Match_Reg(lexem, IDENTIFICATOR)) {
		token->type = Identificator;

		//cout << " IDENTIFICATOR" << endl;
	}
	else  if ((lexem == "+") || (lexem == "-") || (lexem == "*") ||
		(lexem == "/") || (to_lower(lexem) == "mod") || (to_lower(lexem) == "div")) {
		token->type = AriphmethicalOperator;

		//cout << " AriphmethicalOperator" << endl;
	}
	else if ((lexem == "(") || (lexem == ")") || (lexem == "}") ||
		(lexem == "{") || (lexem == "[") || (lexem == "]") || (lexem == ":") || (lexem == ";") || (lexem == ".") || (lexem == ",")) {
		token->type = Separator;

		//cout << " Separator" << endl;
	}
	else if (Match_Reg(lexem, STRING) || Match_Reg(lexem, CHAR)) {
		token->type = Literal;

		//cout << " Literal" << endl;
	}
	else if (Match_Reg(lexem, DIGIT)) {
		token->type = Literal;

		//cout << " Literal" << endl;
	}
	else {
		//cout << " ERROR" << endl;
		token->type = ErrorToken;
	}

	token->value = lexem;
	return token;
}

Token::Token(){}

Token::Token(string val)
{
	value = val;
}
