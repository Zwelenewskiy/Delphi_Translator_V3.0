#include "Parser.h"
#include "Lexer.h"
#include "SymbolTable.h";

void Parser::Parse(string path)
{ 
	lexer = new Lexer(path);
	current_token = lexer->GetToken();

	bool correct = true;

	if (to_lower(current_token->value) == "function") {
		correct = parse_subprogramm(Function);
	}
	else if (to_lower(current_token->value) == "procedure") {
		correct = parse_subprogramm(Procedure);
	}
	else if (current_token->value == "var") {
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

			if (!match(new Token(":="), false))
				return false;

			save_state();
			if (!match(Identificator, false)) {
				if (!match(Literal, false))
					return false;
			}
			else {
				load_state();
				if (!current_env->get(current_token) && !global_env->get(current_token)) {
					cout << endl << "TOKEN NOT DEFINED: " << current_token->value << endl;
					return false;
				}

				if (current_token->check_type == Function) {
					if (!parse_call())
						return false;
				}
				else {
					cout << endl << "EXPECTED FUNCTION: " << current_token->value << endl;
					return false;
				}
			}

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
			
			if (current_token->value == ")") {
				if (current_token->value == ";")
					return true;
				else {
					match(new Token(")"));
					continue;
				}
			}

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

			save_state();
			if (!match(Identificator, false)) {
				if (!match(Literal, false))
					return false;
			}
			else {
				load_state();
				if (!current_env->get(current_token) && !global_env->get(current_token)) {
					cout << endl << "TOKEN NOT DEFINED: " << current_token->value << endl;
					return false;
				}

				if (current_token->check_type == Function) {
					if (!parse_call())
						return false;
				}
				else {
					cout << endl << "EXPECTED FUNCTION: " << current_token->value << endl;
					return false;
				}
			}

			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator))
					return false;
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

bool Parser::parse_call()
{
	if (!match(Identificator))
		return false;

	if (!match(new Token("(")))
		return false;

	if (!parse_call_param_list())
		return false;

	if (!match(new Token(")")))
		return false;

	if (current_token->value == ";")
		return true;
	else
		return false;
}

bool Parser::parse_subprogramm(CheckTokenType type)
{
	if (type == Function) {
		if (!match(new Token("function")))
			return false;

		current_token->check_type = Function;
	}
	else if (type == Procedure) {
		if (!match(new Token("procedure")))
			return false;

		current_token->check_type = Procedure;
	}	

	if (current_token->type != Identificator)
		return false;
	
	if (!global_env->get(current_token)) {
		global_env->put(current_token);
	}
	else {
		cout << endl << "TOKEN ALREADY EXIST: " << current_token->value << endl;
		return false;
	}

	if (!match(Identificator))
		return false;

	if (!match(new Token("(")))
		return false;

	if (!parse_param_list())
		return false;

	if (!match(new Token(")")))
		return false;

	if (type == Function) {
		if (!match(new Token(":")))
			return false;

		if (!match(TypeData))
			return false;
	}	

	if (!match(new Token(";")))
		return false;

	if (current_token->value == "var")
		if (!parse_var())
			return false;

	if (current_token->value != "begin")
		return false;

	if (match(new Token("end"), false))
		return true;
	else {
		if (!stmt())
			return false;
	}
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

bool Parser::parse_call_param_list()
{
	while (true) {
		if (!match(Identificator, false)) {
			if (!match(Literal, false))
			{
				if (current_token->value != ")")
					return false;
			}			
		}

		if (current_token->value == ")")
			return true;

		if (!match(new Token(","), false)) {
			return false;
		}
		else
			continue;
	}
}

bool Parser::parse_var(bool global)
{
	Env* env;

	if (global)
		env = global_env;
	else
		env = new Env(Local);

	if (!match(new Token("var")))
			return false;

	if (current_token->type != Identificator) 
		return false;

	while (current_token->value != "begin")
	{
		if (current_token->type == Identificator) {
			current_token->check_type = Var;

			if (global) {
				if (!global_env->get(current_token)) {
					env->put(current_token);
				}
				else {
					cout << endl << "TOKEN ALREADY EXIST: " << current_token->value << endl;
					return false;
				}
			}
			else {
				if(!env->get(current_token) && !global_env->get(current_token)) {
					env->put(current_token);
				}
				else {
					cout << endl << "TOKEN ALREADY EXIST: " << current_token->value << endl;
					return false;
				}					
			}
				
			if (!match(Identificator)) {
				return false;
			}

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

	if (global) 
		current_env = global_env;
	else 
		current_env = env;

	return true;
}

void Parser::save_state()
{
	tmp_current_file_pos = lexer->current_file_pos;
	tmp_current_token = current_token;
}

void Parser::load_state()
{
	lexer->current_file_pos = tmp_current_file_pos;
	current_token = tmp_current_token;
}

bool Parser::stmt()
{
	if (current_token == nullptr) {
		return true;
	}

	if (to_lower(current_token->value) == "function") {
		if (!parse_subprogramm(Function))
			return false;
	}
	else if(to_lower(current_token->value) == "procedure") {
		if (!parse_subprogramm(Procedure))
			return false;
	}
	else if(current_token->value == "var") {
		if (!parse_var(true))
			return false;
	}
	else if (current_token->value == "begin") {
		operator_brackets_balance++;

		if (!match(new Token("begin")))
			return false;

		if (!stmt())
			return false;	

		if (!match(new Token("end"))) {
			return false;
		}
		else {
			operator_brackets_balance--;
		}

		if (match(new Token(";"), false)) {
			return true;
		}
		else if (match(new Token("."))) {
			if (operator_brackets_balance != 0)
				return false;

			if (current_token != nullptr) {
				cout << endl << "EXPECTED END OF FILE" << endl;
				return false;
			}
			else
				return true;
		}
		else
			return false;
	}
	else if (current_token->value == "end") {
		if (operator_brackets_balance == 0)
			return false;

		return true;
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
		save_state();

		if (!current_env->get(current_token) && !global_env->get(current_token)) {
			cout << endl << "TOKEN NOT DEFINED: " << current_token->value << endl;
			return false;
		}

		if (current_token->check_type == Var) {
			if (!parse_expr())
				return false;
		}
		else if ((current_token->check_type == Function) ||
				(current_token->check_type == Procedure)) {
			if (!parse_call()) 
				load_state();
		}	

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

		if (current_token == nullptr) {
			cout << endl << "EXPECTED TOKEN TYPE: " << token_type << " but found the end of the file" << endl;
			return false;
		}
		return true;
	}
	else if (show_error) {
		cout << endl << "EXPECTED TOKEN TYPE: " << token_type << endl;
		return false;
	}
}

Parser::Parser(){ operator_brackets_balance = bracket_balance = 0; }

Parser::~Parser(){}
