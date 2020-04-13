#include "Parser.h"
#include "Lexer.h"

void Parser::Parse(string path)
{
	lexer = new Lexer(path);
	current_token = lexer->GetToken();

	bool correct = true;

	if (current_token->value == "function") {
		correct = parse_function();
	}
	else if (current_token->value == "procedure") {
		correct = parse_procedure();
	}
	else
	if (current_token->value != "var") {
			correct = parse_var();
	}
	else if (current_token->value != "begin") {
		correct = false;
	}
	
	while ((current_token != nullptr) && correct) {
		correct = stmt();
	}

	if(correct)
		cout << endl << "SYNTAX IS OK" << endl;
	else
		cout << endl << "SYNTAX ERROR" << endl;

	/*Token* token; 
	while (true) {
		token = lexer->GetToken();
		if (token == nullptr)
			break;

		cout << token->value << endl;
	}*/
}

bool Parser::parse_expr()
{
	while ((current_token->value != ";") && (current_token->value != "else")) {
		if (current_token->type == Identificator) {
			if (!match(Identificator))
				return false;

			if (current_token->value == ";")
				return true;

			if (current_token->value == "else")
				return true;

			if (!match(LogicalOperator, false)) {
				if(!match(AriphmethicalOperator))
					return
						false;
			}

			if (!parse_expr())
				return false;
		}
		else if (current_token->type == LogicalOperator) {
			if (!match(LogicalOperator))
				return false;

			if (!match(Identificator))
				return false;

			if (!parse_expr())
				return false;
		}
		else if (current_token->type == AriphmethicalOperator) {
			if (!match(AriphmethicalOperator))
				return false;

			if (!match(Identificator))
				return false;

			if (!parse_expr())
				return false;
		}
		else if (current_token->type == Literal) {
			if (!match(Literal))
				return false;

			if (current_token->value == ";")
				return true;

			if (current_token->value == "else")
				return true;

			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator))
				return
					false;
			}

			if (!parse_expr())
				return false;
		}
		else if (current_token->value == "(") {
			if (!match(new Token("(")))
				return false;

			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator))
					return
					false;
			}

			if (!parse_expr())
				return false;
		}
		else if (current_token->value == ")") {
			if (!match(new Token(")")))
				return false;

			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator))
					return
					false;
			}

			if (!parse_expr())
				return false;
		}
		else
			return false;
	}

	return true;
}

bool Parser::parse_bool_expr()
{
	bracket_balance = 1;
	while (bracket_balance != 0) {
		Token* pred_token = current_token;

		if (bracket_balance == 0) {
			current_token = pred_token;
			return true;
		}

		if (current_token->type == Identificator) {
			if (!match(Identificator))
				return false;

			if (current_token->value == ")") {
				bracket_balance--;

				if (bracket_balance == 0)
					return true;
			}

			if (!match(LogicalOperator, false))
				return false;

			if (current_token->value == "(")
				bracket_balance++;

			if (!parse_bool_expr())
				return false;
		}
		else
			return false;
	}
}

bool Parser::parse_function()
{
	if (!match(new Token("function")))
		return false;

	if (!match(Identificator))
		return false;

	if (!match(new Token("(")))
		return false;

	if (!parse_param_list())
		return false;

	if (!match(new Token(")")))
		return false;

	if (!match(new Token(":")))
		return false;

	if (!match(TypeData))
		return false;

	if (!match(new Token(";")))
		return false;

	if (current_token->value == "var")
		if (!parse_var())
			return false;

	if (current_token->value != "begin")
		return false;
	
	if (!stmt())
		return false;
}

bool Parser::parse_procedure()
{
	if (!match(new Token("procedure")))
		return false;

	if (!match(Identificator))
		return false;

	if (!match(new Token("(")))
		return false;

	if (!parse_param_list())
		return false;

	if (!match(new Token(")")))
		return false;

	if (!match(new Token(";")))
		return false;

	if (current_token->value == "var")
		if (!parse_var())
			return false;

	if (current_token->value != "begin")
		return false;

	if (!stmt())
		return false;
}

bool Parser::parse_param_list()
{
	while (true) {
		if (!match(Identificator, false)) {
			if (current_token->value != ")")
				return false;
			else
				return true;
		}

		if (!match(new Token(","), false)) {
			if (match(new Token(":"))) {
				if (!match(TypeData))
					return false;
			}
		}
		else
			continue;

		if (!match(new Token(";"), false)) {
			if (current_token->value != ")")
				return false;
			else
				return true;
		}			
	}
}

bool Parser::parse_var()
{
	if (!match(new Token("var")))
			return false;

	if (current_token->type != Identificator)
		return false;

	while (current_token->value != "begin")
	{
		if (current_token->type == Identificator) {
			if (!match(Identificator))
				return false;

			if (!match(new Token(","), false)) {
				if (match(new Token(":"))) {
					if (match(TypeData)) {
						if (!match(new Token(";"))) {
							return false;
						}
						else
							continue;
					}
				}
				else
					return false;
			}			
		}		
	}

	return true;
}

bool Parser::stmt()
{
	if (current_token == nullptr) {
		return true;
	}

	if (current_token->value == "function") {
		if (!parse_function())
			return false;
	}
	else if(current_token->value == "procedure") {
		if (!parse_procedure())
			return false;
	}
	else if(current_token->value == "var") {
		if (!parse_var())
			return false;
	}
	else if (current_token->value == "begin") {
		if (!match(new Token("begin")))
			return false;

		if (!stmt())
			return false;	
	}
	else if (current_token->value == "end") {
		if (match(new Token("end"))) {
			if (match(new Token(";"), false)) {
				return true;
			}
			else if (match(new Token("."))) {
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}		
	else if (current_token->value == "var") {
		if (!parse_var())
			return false;
	}
	else if (current_token->value == "if") {
		if (!match(new Token("if")))
			return false;

		if (!match(new Token("("))) 
			return false;

		if (!parse_bool_expr())
			return false;

		if (!match(new Token(")")))
			return false;

		if (!match(new Token("then")))
			return false;

		if (!stmt())
			return false;
	}
	else  if (current_token->type == Identificator) {
		if (!match(Identificator))
			return false;

		if (!match(new Token(":=")))
			return false;

		if (!parse_expr())
			return false;

		if(match(new Token("else"), false)){
			if (!stmt())
				return false;
		}
		else if (!match(new Token(";"))) 			
			return false;
		else if (!stmt())
			return false;
	}
	else if (current_token->value == "while") {
		if (!match(new Token("while")))
			return false;

		if (!match(new Token("(")))
			return false;

		if (!parse_bool_expr())
			return false;

		if (!match(new Token(")")))
			return false;

		if (!match(new Token("do")))
			return false;

		if (!stmt())
			return false;
	}
	else {
		return false;
	}

	return true;
}

bool Parser::match(Token* token, bool show_error)
{
	if (token->value == current_token->value) {
		current_token = lexer->GetToken();
		return true;
	}
	else if(show_error){
		cout << endl << "EXPECTED TOKEN: " << token->value << endl;
		return false;
	}
}

bool Parser::match(TokenType token_type, bool show_error)
{
	if (token_type == current_token->type) {
		current_token = lexer->GetToken();
		return true;
	}
	else if (show_error) {
		cout << endl << "EXPECTED TOKEN TYPE: " << token_type << endl;
		return false;
	}
}

Parser::Parser(){}

Parser::~Parser(){}