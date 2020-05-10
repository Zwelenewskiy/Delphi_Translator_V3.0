#include "Parser.h"

void Parser::Parse(string path, Node* tree)
{ 
	lexer = new Lexer(path);
	current_token = lexer->GetToken();

	bool correct = true;
	if (match(new Token("program"))) {
			correct = match(Identificator);
			correct = match(new Token(";"));
	}

	if (to_lower(current_token->value) == "type") {

		match(new Token("type"));

		while ((current_token->value != "function")
			&& (current_token->value != "procedure")
			&& (current_token->value != "var")
			&& (current_token->value != "begin")) {
			if (!parse_struct(tree)) {
				correct = false;
				break;
			}
		}
	}
	else if (to_lower(current_token->value) == "function") {
		correct = parse_subprogramm(Function, tree);
	}
	else if (to_lower(current_token->value) == "procedure") {
		correct = parse_subprogramm(Procedure, tree);
	}
	else if (current_token->value == "var") {
		current_modifier = Public;
		correct = parse_var(tree);
	}
	else if (current_token->value != "begin") {
		correct = false;
	}
	
	while ((current_token != nullptr) && correct) {
		correct = stmt(tree);
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

		cout << token->value << " " << token->line << endl;
	}*/
}

Node* Parser::parse_expr(Node* tree)
{
	Token* parent = nullptr;

	BuildingTree* builder_tree = new BuildingTree();
	while ((current_token->value != ";") && (current_token->value != "else")) {
		builder_tree->create_ast_tree(current_token, AriphmethicalExpr);

		Token* tmp = current_token;		

		if (current_token->type == Identificator) {
			global_env->get(tmp, false);

			if (!match(Identificator))
				return false;

			if (current_token->value == ".") {
				if (tmp->data_type == UserDataType) {
					for (Token* token : user_datatypes) {
						if (token->value == tmp->parent->value) {
							parent = token;
							break;
						}
					}

					continue;
				}
				else {
					ShowError("TOKEN IS NOT A STRUCT: " + tmp->value);
					return false;
				}
			}

			if ((current_token->type == AriphmethicalOperator)
				|| (current_token->type == AriphmethicalOperator)) 
			{
				continue;
			}

			if (!match(new Token(":="), false)) {
				if (current_token->value != ";") {
					ShowError("EXPECTED ; BUT " + current_token->value);
					return false;
				}
				else
					return tree;
			}
			else
				continue;

			save_state();
			if (!match(Identificator, false)) {
				if (!match(Literal, false))
					return false;
			}
			else {
				load_state();
				if (!global_env->get(current_token, false)) {
					ShowError("TOKEN NOT DEFINED: " + current_token->value);
					return false;
				}

				if (current_token->check_type == Function) {
					if (!parse_call(current_token, tree)) {
						ShowError("EXPECTED FUNCTION OR PROCEDURE");
						return false;
					}
				}
				/*else {
					cout << endl << "EXPECTED FUNCTION: " << current_token->value << endl;
					return false;
				}*/
			}

			if (current_token->value == ";")
				return tree;

			if (current_token->value == "else")
				return tree;

			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator))
					return false;
			}

			if (!parse_expr(tree)) {
				ShowError("EXPECTED EXPRESSION");
				return false;
			}
		}
		else if (current_token->value == ".") {
			match(new Token("."));

			if (current_token->type != Identificator) {
				ShowError("EXPECTED IDENTIFICATOR  BUT " + current_token->type);
				return false;
			}

			CheckTokenType type = UndefinedCheckTokenType;			
			
			for (Token* var : parent->members->table) {
				if (var->value == current_token->value) {
					if (var->modifier == Public) {
						current_token = var;
						type = var->check_type;
						break;
					}
					else {
						cout << endl << "TOKEN IS NOT PUBLIC: " << var->value << endl;
						return false;
					}
				}
			}

			if (type == UndefinedCheckTokenType) {
				cout << endl << "TOKEN " << current_token->value << " IS NOT A MEMBER "<< parent->value << endl;
				return false;
			}
			else if ((type == Procedure) || (type == Function)) {
				if (!parse_call(current_token, tree)) {
					ShowError("EXPECTED PROCEDURE OR FUNCTION");
					return false;
				}
			}
			else if (type == Var) {
				tmp = current_token;
				match(current_token);

				if (current_token->value == ":=") {
					match(current_token);
					continue;
				}
				else if (current_token->value == ".") {
					parent = tmp->parent;

					if (tmp->data_type == UserDataType) {
						continue;
					}
					else {
						ShowError("TOKEN IS NOT A STRUCT: " + tmp->value);
						return false;
					}
				}
				else if (current_token->value == ";") {
					return tree;
				}
				else {
					ShowError("EXPECTED PROCEURE, FUNCTION OR VARIABLE BUT " + current_token->type);
					return false;
				}
			}
		}
		else if (current_token->type == LogicalOperator) {
			if (!match(LogicalOperator)) {
				ShowError("EXPECTED LOGICAL OPERATATOR BUT " + current_token->type);
				return false;
			}

			if (!match(Identificator)) {
				ShowError("EXPECTED IDENTIFICATOR BUT " + current_token->type);
				return false;
			}

			if (!parse_expr(tree)) {
				ShowError("EXPECTED EXORISSION BUT");
				return false;
			}
		}
		else if (current_token->type == AriphmethicalOperator) {
			if (!match(AriphmethicalOperator)) {
				ShowError("EXPECTED ARIPHMETHICAL OPERTATOR BUT" + current_token->type);
				return false;
			}

			if (!match(Identificator, false)) {
				if (!match(Literal)) {
					return false;
				}
			}				

			continue;
		}
		else if (current_token->type == Literal) {
			if (!match(Literal))
				return false;

			if (current_token->value == ";")
				return tree;

			if (current_token->value == "else")
				return tree;
			
			if (current_token->value == ")") {
				if (current_token->value == ";")
					return tree;
				else {
					match(new Token(")"));
					continue;
				}
			}

			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator)) {
					ShowError("EXPECTED ARIPHMETHICAL OPERTATOR BUT" + current_token->type);
					return false;
				}
			}

			if (!parse_expr(tree)) {
				ShowError("EXPECTED EXPRESSION");
				return false;
			}
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
					ShowError("TOKEN NOT DEFINED: " + current_token->value);
					return false;
				}

				if (current_token->check_type == Function) {
					if (!parse_call(current_token, tree)) {
						ShowError("EXPECTED LOGICAL OPERTATOR BUT" + current_token->type);
						return false;
					}
				}
				else {
					cout << endl << "EXPECTED FUNCTION: " << current_token->value << endl;
					ShowError("EXPECTED FUNCTION BUT " + current_token->type);
					return false;
				}
			}

			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator)) {
					ShowError("EXPECTED ARIPHMETHICAL OPERTATOR BUT" + current_token->type);
					return false;
				}
			}

			if (!parse_expr(tree)) {
				ShowError("EXPECTED AEXPRESSION");
				return false;
			}
		}
		else if (current_token->value == ")") {
			if (!match(new Token(")")))
				return false;

			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator)) {
					ShowError("EXPECTED ARIPHMETHICAL OPERTATOR BUT" + current_token->type);
					return false;
				}
			}

			if (!parse_expr(tree)) {
				ShowError("EXPECTED EXPRESSION" + current_token->type);
				return false;
			}
		}
		else {
		ShowError("ERROR TOKEN  " + current_token->value);
			return false;
		}

		if((current_token->value != ";") && (current_token->value != "else"))
			return builder_tree->create_ast_tree(current_token, AriphmethicalExpr, true);
	}

	return tree;
}

Node* Parser::parse_bool_expr(Node* tree)
{
	bracket_balance = 1;
	BuildingTree* builder_tree = new BuildingTree();
	while (bracket_balance != 0) {
		builder_tree->create_ast_tree(current_token, AriphmethicalExpr);

		Token* pred_token = current_token;

		if (bracket_balance == 0) {
			current_token = pred_token;
			return tree;
		}

		if (current_token->type == Identificator) {
			if (!match(Identificator)) 
				return false;

			if (current_token->value == ")") {
				bracket_balance--;

				if (bracket_balance == 0)
					return tree;
			}

			if (!match(LogicalOperator, false))
				return false;

			if (current_token->value == "(")
				bracket_balance++;

			if (!parse_bool_expr(tree)) {
				ShowError("EXPECTED BOOLEAN EXPRESSION");
				return false;
			}
		}
		else {
			ShowError("EXPECTED IDENTIFICATOR BUT " + current_token->value);
			return false;
		}

		if(bracket_balance == 0)
			return builder_tree->create_ast_tree(current_token, AriphmethicalExpr, true);
	}
}

Node* Parser::parse_call(Token* subprogram_token, Node* tree)
{
	Token* tmp = current_token;
	if (!match(Identificator))
		return false;

	if (!match(new Token("(")))
		return false;

	vector<Variable> signature;
	if (!parse_call_param_list(signature, tree)) {
		ShowError("BAD SIGNATURE PARSING");
		return false;
	}
	
	if (subprogram_token->parent == nullptr) {
		if (!global_env->check_signature(subprogram_token, signature)) {
			ShowError("PARAMETERS DON'T MATCH THE SIGNATURE: " + tmp->value);
			return false;
		}
	}
	else {
		if (!global_env->check_signature(subprogram_token, signature, subprogram_token->parent->members)) {
			ShowError("PARAMETERS DON'T MATCH THE SIGNATURE: " + tmp->value);
			return false;
		}
	}


	if (!match(new Token(")")))
		return false;

	if ((current_token->value == ";") || (current_token->type == AriphmethicalOperator))
		return tree;
	else {
		ShowError("EXPECTED ; OR ARIPHMETHICAL OPERATOR");
		return false;
	}
}

Node* Parser::parse_subprogramm(CheckTokenType type, Node* tree, bool global)
{
	if(global)
		current_env = new Env();

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

	Token* subprogramm_token = current_token;

	if (current_token->type != Identificator) {
		ShowError("EXPECTED IDENTIFICATOR");
		return false;
	}

	if (!match(Identificator))
		return false;

	if (!match(new Token("(")))
		return false;
	
	vector<Variable> signature;
	if (!parse_param_list(signature, tree)) {
		ShowError("BAD SIGNATURE PARSING");
		return false;
	}

	subprogramm_token->signature = signature;
	subprogramm_token->modifier = current_modifier;

	if (global) {
		if (!global_env->get(subprogramm_token)) {
			global_env->put(subprogramm_token);
		}
		else {
			ShowError("TOKEN ALREADY EXIST: " + subprogramm_token->value);
			return false;
		}
	}
	else {
		if (!current_env->get(subprogramm_token)) {
			subprogramm_token->parent = current_struct;
			current_env->put(subprogramm_token);
		}
		else {
			ShowError("TOKEN ALREADY EXIST: " + subprogramm_token->value);
			return false;
		}
	}	

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
		if (!parse_var(tree, true, false)) {
			ShowError("BAD VARIABLE DECLARATION PARSING");
			return false;
		}

	if (current_token->value != "begin") {
		ShowError("EXPECTED begin BUT " + current_token->value);
		return false;
	}

	if (match(new Token("end"), false))
		return tree;
	else {
		if (!stmt(tree))
			return false;
	}
}

Node* Parser::parse_param_list(vector<Variable>& signature, Node* tree)
{
	Token* tmp_token;

	while (true) {
		vector<Token*> tmp_vars;
		tmp_token = current_token;

		if (!match(Identificator, false)) {
			if (current_token->value != ")")
				return false; 
			else
				return tree;
		}
		else {
			if (!current_env->get(tmp_token)) { 
				tmp_token->check_type = Var;
				current_env->put(tmp_token);

				tmp_vars.push_back(tmp_token);
			}
			else {
				ShowError("TOKEN IS NOT DEFINED: " + tmp_token->value);
				return false;
			}
		}		

		if (!match(new Token(","), false)) {
			if (match(new Token(":"))) {
				Token* data_type = current_token;
				if (!match(TypeData))
					return false;
				else {
					for (int i = 0; i < tmp_vars.size(); i++) {
						if(data_type->value == "integer")
							signature.push_back(Variable(tmp_vars[i]->value, Integer));
						else if (data_type->value == "boolean")
							signature.push_back(Variable(tmp_vars[i]->value, Boolean));
						else if (data_type->value == "char")
							signature.push_back(Variable(tmp_vars[i]->value, Char));
						else if (data_type->value == "double")
							signature.push_back(Variable(tmp_vars[i]->value, Double));
						else if (data_type->value == "string")
							signature.push_back(Variable(tmp_vars[i]->value, String));
					}
				}
			}
		}
		else
			continue;

		if (!match(new Token(";"), false)) {
			if (current_token->value != ")") {
				ShowError("EXPECTED TOKEN ( BUT : " + current_token->value);
				return false;
			}
			else
				return tree;
		}			
	}
}

Node* Parser::parse_call_param_list(vector<Variable>& signature, Node* tree){
	;
	while (true) {
		Token* tmp = current_token;
		if (!match(Identificator, false)) {
			tmp = current_token;
			if (!match(Literal, false))
			{
				if (current_token->value != ")") {
					ShowError("EXPECTED TOKEN ) BUT: " + current_token->value);
					return false;
				}
			}			
			else {
				signature.push_back(Variable(tmp->value, define_data_type(tmp)));
			}
		} 
		else {
			if (global_env->get(tmp, false)) {
				current_env->put(tmp);
				signature.push_back(Variable(tmp->value, tmp->data_type)); 
			}
			else {
				ShowError("TOKEN IS NOT DEFINED: " + tmp->value);
				return false;
			}
		}

		if (current_token->value == ")")
			return tree;

		if (!match(new Token(","), false)) {
			return false;
		}
		else
			continue;
	}
}

Node* Parser::parse_var(Node* tree, bool global, bool in_struct)
{
	Env* env;
	Token* tmp_token;

	if (global)
		env = global_env;
	else
		current_env = new Env();
	
	if (!in_struct) {
		if (!match(new Token("var")))
			return false;
	}	

	if (current_token->type != Identificator) {
		ShowError("EXPECTED IDENTIFICATOR BUT: " + current_token->type);
		return false;
	}
	
	vector<Token*> tmp_vars;
	bool new_vars = true;
	while (true)
	{
		if (!in_struct) {
			if (current_token->value == "begin")
				break;
		}
		else {
			if ((current_token->value == "end")
				|| (current_token->value == "procedure")
				|| (current_token->value == "function"))
				break;
		}

		if ((to_lower(current_token->value) == "private")
			|| (to_lower(current_token->value) == "public")
			|| (to_lower(current_token->value) == "protected")) 
		{
			if (new_vars) {
				if (to_lower(current_token->value) == "private")
					current_modifier = Private;

				if (to_lower(current_token->value) == "public")
					current_modifier = Public;

				if (to_lower(current_token->value) == "protected")
					current_modifier = Protected;

				match(new Token(current_token->value));
			}
			else {
				cout << endl << "EXPECTED IDENTIFICATOR BUT: " << current_token->value << endl;
				return false;
			}
		}

		if (in_struct) {
			if ((current_token->value == "end")
				|| (current_token->value == "procedure")
				|| (current_token->value == "function"))
				break;
		}

		if (current_token->type == Identificator) {
			current_token->check_type = Var;

			if (global) {
				if (!global_env->get(current_token)) {
					for (Token* t : tmp_vars) {
						if (t->value == current_token->value) {
							ShowError("TOKEN ALREADY EXIST: " + current_token->type);
							return false;
						}
					}

					tmp_vars.push_back(current_token);
				}
				else {
					ShowError("TOKEN ALREADY EXIST: " + current_token->type);
					return false;
				}
			}
			else {
				if (!current_env->get(current_token, false) && !global_env->get(current_token, false)) {
					for (Token* t : tmp_vars) {
						if (t->value == current_token->value) {
							ShowError("TOKEN ALREADY EXIST: " + current_token->type);
							return false;
						}
					}

					tmp_vars.push_back(current_token);
				}
				else {
					ShowError("TOKEN ALREADY EXIST: " + current_token->type);
					return false;
				}
			}

			if (!match(Identificator)) {
				return false;
			}

			if (!match(new Token(","), false)) {
				if (match(new Token(":"))) {
					Token* data_type = current_token;

					if (!match(TypeData, false)) {
						if (!is_user_datatype(current_token)) {
							ShowError("EXPECTED DATATYPE BUT: " + current_token->type);
							return false;
						}
						else {
							data_type = current_token;
							match(current_token);
							if (!match(new Token(";"))) {
								return false;
							}
							else {
								for (int i = 0; i < tmp_vars.size(); i++) {

									tmp_vars[i]->data_type = UserDataType;
									tmp_vars[i]->modifier = current_modifier;

									global_env->get(data_type);
									tmp_vars[i]->parent = data_type;

									if (global)
										global_env->put(tmp_vars[i]);
									else
										current_env->put(tmp_vars[i]);

									new_vars = true;
								}

								tmp_vars.clear();
								continue;
							}
						}
					}
					else {
						if (!match(new Token(";"))) {
							return false;
						}
						else {
							for (int i = 0; i < tmp_vars.size(); i++) {

								if (data_type->value == "integer")
									tmp_vars[i]->data_type = Integer;
								else if (data_type->value == "boolean")
									tmp_vars[i]->data_type = Boolean;
								else if (data_type->value == "char")
									tmp_vars[i]->data_type = Char;
								else if (data_type->value == "double")
									tmp_vars[i]->data_type = Double;
								else if (data_type->value == "string")
									tmp_vars[i]->data_type = String;

								tmp_vars[i]->modifier = current_modifier;

								if (global)
									global_env->put(tmp_vars[i]);
								else
									current_env->put(tmp_vars[i]);

								new_vars = true;
							}

							tmp_vars.clear();
							continue;
						}					
					}
				}
				else
					return false;
			}
		}
		else {
			ShowError("EXPECTED IDENTIFICATOR BUT: " + current_token->type);
			return false;
		}
	}

	return tree;
}

Node* Parser::parse_struct(Node* tree)
{
	CheckTokenType type;

	Token* tmp = current_token;
	current_struct = current_token;
	if (!match(Identificator)) {
		return false;
	}
	else {
		if (global_env->get(tmp, false)) {
			ShowError("TOKEN ALREADY  EXIST: " + tmp->value);
			return false;
		}
	}

	if (!match(new Token("=")))
		return false;

	if (!match(new Token("record"), false)) {
		if (!match(new Token("class"))) {
			ShowError("EXPECTED CLASS OR RECORD BUT: " + current_token->value);
			return false;
		}
		else
			type = Class;
	}
	else
		type = Record;

	if (current_token->type == Identificator) {
		if (!parse_var(false, true)) {
			ShowError("BAD VARIABLE  DECLARATION PARSING");
			return false;
		}
	}

	if (to_lower(current_token->value) == "private") {
		current_modifier = Private;
		match(new Token(current_token->value));
	}
	else if (to_lower(current_token->value) == "public") {
		current_modifier = Public;
		match(new Token(current_token->value));
	}
	else if (to_lower(current_token->value) == "protected") {
		current_modifier = Protected;
		match(new Token(current_token->value));
	}

	if (current_token->type == Identificator) {
		if (!parse_var(false, true)) {
			ShowError("BAD VARIABLE  DECLARATION PARSING");
			return false;
		}
	}

	while (current_token->value != "end") 
	{
		if (to_lower(current_token->value) == "private") {
			current_modifier = Private;
			match(new Token(current_token->value));
		}
		else if (to_lower(current_token->value) == "public") {
			current_modifier = Public;
			match(new Token(current_token->value));
		}
		else if (to_lower(current_token->value) == "protected") {
			current_modifier = Protected;
			match(new Token(current_token->value));
		}

		if (current_token->value == "function") {
			if (!parse_subprogramm(Function, false)) {
				ShowError("BAD FUNCTION PARSING");
				return false;
			}
		}
		else if (current_token->value == "procedure") {
			if (!parse_subprogramm(Procedure, false)) {
				ShowError("BAD PROCEDURE PARSING");
				return false;
			}
		}
		else {
			ShowError("EXPECTED FUNCTION OR PROCEDURE DECLARATION BUT: " + current_token->value);
			return false;
		}
	}

	match(new Token("end"));
	if (!match(new Token(";"))) {
		return false;
	}
	else {
		tmp->check_type = type;
		tmp->members = current_env;

		global_env->put(tmp);
		user_datatypes.push_back(tmp);

		return tree;
	}
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

bool Parser::is_user_datatype(Token*& token)
{
	for (Token* t : user_datatypes) {
		if (token->value == t->value) {
			token = t;
			return true;
		}
	}

	return false;
}

DataTypes Parser::define_data_type(Token * token)
{
	if (Match_Reg(token->value, CHAR))
		return Char;

	if (Match_Reg(token->value, STRING))
		return String;

	if ((to_lower(token->value) == "true") || (to_lower(token->value) == "false"))
		return Boolean;

	if (Match_Reg(token->value, INTEGER))
		return Integer;

	if (Match_Reg(token->value, DIGIT))
		return Double;

	return DataTypes();
}

Node* Parser::stmt(Node* tree)
{
	if (current_token == nullptr) {
		return tree;
	}

	if (to_lower(current_token->value) == "type") {

		match(new Token("type"));

		while ((current_token->value != "function")
			&& (current_token->value != "procedure")
			&& (current_token->value != "var")
			&& (current_token->value != "begin")) {
			if (!parse_struct(tree)) {
				ShowError("BAD RECORD OR CLASS PARSING");
				return false;
			}
		}
	}
	else if (to_lower(current_token->value) == "function") {
		if (!parse_subprogramm(Function, tree)) {
			ShowError("BAD FUNCTION PARSING");
			return false;
		}
	}
	else if(to_lower(current_token->value) == "procedure") {
		if (!parse_subprogramm(Procedure, tree)) {
			ShowError("BAD PROCEDURE PARSING");
			return false;
		}
	}
	else if(current_token->value == "var") {
		current_modifier = Public;

		if (!parse_var(tree, true)) {
			ShowError("BAD VARIABLE DECLARATION PARSING");
			return false;
		}
	}
	else if (current_token->value == "begin") {
		operator_brackets_balance++;

		if (!match(new Token("begin")))
			return false;

		if (!stmt(tree))
			return false;	

		if (!match(new Token("end"))) {
			return false;
		}
		else {
			operator_brackets_balance--;
		}

		if (match(new Token(";"), false)) {
			return tree;
		}
		else if (match(new Token("."))) {
			if (operator_brackets_balance != 0)
				return false;

			if (current_token != nullptr) {
				ShowError("EXPECTED END OF FILE");
				return false;
			}
			else
				return tree;
		}
		else
			return false;
	}
	else if (current_token->value == "end") {
		if (operator_brackets_balance == 0) {
			ShowError("BAD OPERATOR BRACKETS  BALANCE");
			return false;
		}

		return tree;
	}
	else if (current_token->value == "var") {
		current_modifier = Public;

		if (!parse_var(tree)) {
			ShowError("BAD VARIABLE DECLARATION PARSING");
			return false;
		}
	}
	else if (current_token->value == "if") {
		if (!match(new Token("if")))
			return false;

		if (!match(new Token("("))) 
			return false;

		if (!parse_bool_expr(tree))
			return false;

		if (!match(new Token(")")))
			return false;

		if (!match(new Token("then")))
			return false;

		if (!stmt(tree)) {
			ShowError("EXPECTED EXPRESSION");
			return false;
		}
	}
	else  if (current_token->type == Identificator) {
		save_state();

		if (!global_env->get(current_token, false) && !current_env->get(current_token, false)) {
			ShowError("TOKEN NOT DEFINED: " + current_token->value);
			return false;
		}

		if (current_token->check_type == Var) {
			if (!parse_expr(tree)) {
				ShowError("EXPECTED EXPRESSION");
				return false;
			}
		}
		else if ((current_token->check_type == Function) ||
				(current_token->check_type == Procedure)) {
			if (!parse_call(current_token, tree))
				load_state();
		}	

		if(match(new Token("else"), false)){
			if (!stmt(tree)) {
				ShowError("EXPECTED EXPRESSION");
				return false;
			}
		}
		else if (!match(new Token(";"))) 			
			return false;
		else if (!stmt(tree)) {
			ShowError("EXPECTED EXPRESSION");
			return false;
		}
	}
	else if (current_token->value == "while") {
		if (!match(new Token("while")))
			return false;

		if (!match(new Token("(")))
			return false;

		if (!parse_bool_expr(tree))
			return false;

		if (!match(new Token(")")))
			return false;

		if (!match(new Token("do")))
			return false;

		if (!stmt(tree))
			return false;
	}
	else {
		ShowError("ERROR  TOKEN: " + current_token->value);
		return false;
	}

	return tree;
}

bool Parser::match(Token* token, bool show_error)
{
	if (to_lower(token->value) == to_lower(current_token->value)) {
			current_token = lexer->GetToken();
			return true;
	}
	else if(show_error){
		ShowError("EXPECTED TOKEN: " + token->value);
		return false;
	}
}

bool Parser::match(TokenType token_type, bool show_error)
{
	if (token_type == current_token->type) {
		current_token = lexer->GetToken();

		if (current_token == nullptr) {
			string tmp = "EXPECTED TOKEN TYPE: " + token_type;
			tmp += " BUT FOUND END OF THE FILE";
			ShowError(tmp);
			return false;
		}
		return true;
	}
	else if (show_error) {
		ShowError("EXPECTED TOKEN: " + token_type);
		return false;
	}
}

Parser::Parser(){ 
	operator_brackets_balance = bracket_balance = 0; 
	current_modifier = Public;
}

Parser::~Parser(){}
