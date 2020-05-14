#include "Parser.h"

void Parser::Parse(string path, Node*& tree)
{ 
	lexer = new Lexer(path);
	current_token = lexer->GetToken();
	bool correct = true;

	if (match(new Token("program"))) {
		if (!match(Identificator))
			tree = nullptr;

		if(!match(new Token(";")))
			tree = nullptr;
	}

	if (to_lower(current_token->value) == "type") {

		match(new Token("type"));

		bool first = true;
		while ((current_token->value != "function")
			&& (current_token->value != "procedure")
			&& (current_token->value != "var")
			&& (current_token->value != "begin"))
		{	

			if (first) {
				first = false;
				tree = parse_struct();
			}			
			else {
				Node* tmp = new Node();

				if(!tree->next)
					tree->next = parse_struct();
				else {
					while (tmp->next)
						tmp = tmp->next;

					tmp->next = parse_struct();
				}				
			}

			/*tree = parse_struct();
			if (!tree) {
				ShowError("EXPECTED TYPE");
				correct = false;
				break;
			}*/
		}
	}
	else if (to_lower(current_token->value) == "function") {
		tree = parse_subprogramm(Function);
	}
	else if (to_lower(current_token->value) == "procedure") {
		tree = parse_subprogramm(Procedure);
	}
	else if (current_token->value == "var") {
		current_modifier = Public;
		tree = parse_var();
	}
	else if (current_token->value != "begin") {
		tree = false;
	}
	
	while (current_token && tree && correct) {
		Node* node = new Node();
		if (!tree->next) {
			tree->next = stmt();
		}
		else {
			node = tree->next;
			while (node->next)
				node = node->next;

			node->next = stmt();

			if (!node->next)
				correct = false;
		}
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

Node* Parser::parse_expr()
{
	Token* parent = nullptr;
	Node* node = new Node();

	BuildingTree* builder_tree = new BuildingTree();
	while ((current_token->value != ";") && (current_token->value != "else")) {

		Token* tmp = current_token;		

		tmp = current_token;
		if (current_token->type == Identificator) {
			global_env->get(tmp);

			builder_tree->infix_to_postfix(tmp, AriphmethicalExpr);

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
				builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);
				continue;
			}
			
			if (!match(new Token(":="), false)) {
				if (current_token->value != ";") {
					ShowError("EXPECTED ; BUT " + current_token->value);
					return false;
				}
				else {
					builder_tree->infix_to_postfix(nullptr, AriphmethicalExpr, true);					

					return builder_tree->build_tree();
				}
			}
			else {
				builder_tree->infix_to_postfix(new Token(":="), AriphmethicalExpr);
				continue;
			}

			save_state();
			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);
			if (!match(Identificator, false)) {
				if (!match(Literal, false))
					return false;
			}
			else {
				load_state();
				if (!global_env->get(current_token)) {
					ShowError("TOKEN NOT DEFINED: " + current_token->value);
					return false;
				}

				if ((current_token->check_type == Function)
					|| (current_token->check_type == Function)) 
				{
					builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);

					node->left = parse_call(current_token);
					if (!node->left) {
						ShowError("EXPECTED FUNCTION OR PROCEDURE");
						return false;
					}
				}
			}

			if (current_token->value == ";") {
				builder_tree->infix_to_postfix(nullptr, AriphmethicalExpr, true);
				return builder_tree->build_tree();
			}

			if (current_token->value == "else") {
				builder_tree->infix_to_postfix(nullptr, AriphmethicalExpr, true);
				return builder_tree->build_tree();
			}
			
			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);
			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator))
					return false;
			}

			continue;
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
				if (!parse_call(current_token)) {
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
					return node;
				}
				else {
					ShowError("EXPECTED PROCEURE, FUNCTION OR VARIABLE BUT " + current_token->type);
					return false;
				}
			}
		}
		else if (current_token->type == LogicalOperator) {
			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);

			if (!match(LogicalOperator)) {
				ShowError("EXPECTED LOGICAL OPERATATOR BUT " + current_token->type);
				return false;
			}

			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);
			if (!match(Identificator)) {
				ShowError("EXPECTED IDENTIFICATOR BUT " + current_token->type);
				return false;
			}

			continue;
		}
		else if (current_token->type == AriphmethicalOperator) {
			//builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);

			if (!match(AriphmethicalOperator)) {
				ShowError("EXPECTED ARIPHMETHICAL OPERTATOR BUT" + current_token->type);
				return false;
			}

			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);
			if (!match(Identificator, false)) {
				if (!match(Literal)) {
					return false;
				}
			}				

			continue;
		}
		else if (current_token->type == Literal) {
			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);

			if (!match(Literal))
				return false;

			if (current_token->value == ";") {
				builder_tree->infix_to_postfix(nullptr, AriphmethicalExpr, true);
				return builder_tree->build_tree();
			}

			if (current_token->value == "else") {
				builder_tree->infix_to_postfix(nullptr, AriphmethicalExpr, true);
				return builder_tree->build_tree();
			}
			
			if (current_token->value == ")") {
				if (current_token->value == ";") {
					builder_tree->infix_to_postfix(nullptr, AriphmethicalExpr, true);
					return builder_tree->build_tree();
				}
				else {
					match(new Token(")"));
					continue;
				}
			}

			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);
			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator)) {
					ShowError("EXPECTED ARIPHMETHICAL OPERTATOR BUT" + current_token->type);
					return false;
				}
			}

			continue;
		}
		else if (current_token->value == "(") {
			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);

			if (!match(new Token("(")))
				return false;

			save_state();
			if (!match(Identificator, false)) {
				if (!match(Literal, false))
					return false;
			}
			else {
				load_state();
				builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);

				if (!current_env->get(current_token) && !global_env->get(current_token)) {
					ShowError("TOKEN NOT DEFINED: " + current_token->value);
					return false;
				}

				node->left = parse_call(current_token);
				if (current_token->check_type == Function) {

					if (!node->left) {
						ShowError("EXPECTED LOGICAL OPERTATOR BUT" + current_token->type);
						return false;
					}
				}
				else {
					ShowError("EXPECTED FUNCTION BUT " + current_token->type);
					return false;
				}
			}
			
			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);
			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator)) {
					ShowError("EXPECTED ARIPHMETHICAL OPERTATOR BUT" + current_token->type);
					return false;
				}
			}

			continue;
		}
		else if (current_token->value == ")") {
			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);

			if (!match(new Token(")")))
				return false;

			builder_tree->infix_to_postfix(current_token, AriphmethicalExpr);
			if (!match(LogicalOperator, false)) {
				if (!match(AriphmethicalOperator)) {
					ShowError("EXPECTED ARIPHMETHICAL OPERTATOR BUT" + current_token->type);
					return false;
				}
			}

			continue;
		}
		else {
			ShowError("ERROR TOKEN  " + current_token->value);
			return false;
		}
	}

	builder_tree->infix_to_postfix(nullptr, AriphmethicalExpr, true);
	return builder_tree->build_tree();
}

Node* Parser::parse_bool_expr()
{
	Node* node = new Node();

	BuildingTree* builder_tree = new BuildingTree();

	bracket_balance = 1;
	while (bracket_balance != 0) {
		builder_tree->infix_to_postfix(current_token, BooleanExpr);

		Token* pred_token = current_token;

		if (bracket_balance == 0) {
			current_token = pred_token;

			builder_tree->infix_to_postfix(nullptr, BooleanExpr, true);
			return builder_tree->build_tree();
		}

		if ((current_token->type == Identificator) || (current_token->type == Literal)) {
			if (!match(Identificator))
				return false;

			if (current_token->value == ")") {
				bracket_balance--;

				if (bracket_balance == 0) {
					builder_tree->infix_to_postfix(nullptr, BooleanExpr, true);
					return builder_tree->build_tree();
				}
			}

			builder_tree->infix_to_postfix(current_token, BooleanExpr);

			if (!match(LogicalOperator, false))
				return false;

			if (current_token->value == "(")
				bracket_balance++;

			continue;
		}
		else {
			ShowError("EXPECTED IDENTIFICATOR BUT " + current_token->value);
			return false;
		}
}
}

Node* Parser::parse_call(Token* subprogram_token)
{
	Node* node = new Node();
	Token* tmp = current_token;
	if (!match(Identificator))
		return false;

	if (!match(new Token("(")))
		return false;

	vector<Variable> signature;
	if (!parse_call_param_list(signature)) {
		ShowError("BAD SIGNATURE PARSING");
		return false;
	}
	
	if (subprogram_token->parent == nullptr) {
		if (!global_env->check_overloads(subprogram_token, signature)) {
			ShowError("PARAMETERS DON'T MATCH THE SIGNATURE: " + tmp->value);
			return false;
		}
	}
	else {
		if (!global_env->check_overloads(subprogram_token, signature, subprogram_token->parent->members)) {
			ShowError("PARAMETERS DON'T MATCH THE SIGNATURE: " + tmp->value);
			return false;
		}
	}


	if (!match(new Token(")")))
		return false;

	if ((current_token->value == ";") || (current_token->type == AriphmethicalOperator))
		return node;
	else {
		ShowError("EXPECTED ; OR ARIPHMETHICAL OPERATOR");
		return false;
	}
}

Node* Parser::parse_subprogramm(CheckTokenType type, bool global)
{
	Node* node = new Node();

	if(global)
		current_env = new Env();

	node->data = current_token;
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
			
	if (!parse_var(false, true, true, signature, true)) {
		ShowError("BAD SIGNATURE PARSING");
		return false;
	}

	subprogramm_token->signature = signature;
	subprogramm_token->modifier = current_modifier;

	node->data->signature = signature;

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
		if (!current_env->get(subprogramm_token, struct_env)) {
			subprogramm_token->parent = current_struct;
			struct_env->put(subprogramm_token);
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

	if (current_token->value == "var") {
		node->left = parse_var(false, false);
		if (!node->left) {
			ShowError("BAD VARIABLE DECLARATION PARSING");
			return false;
		}
	}		
		 
	if (current_token->value != "begin") {
		ShowError("EXPECTED begin BUT " + current_token->value);
		return false;
	}

	if (match(new Token("end"), false))
		return node;
	else {
		node->right = stmt();
		if (!node->right)
			return false;
	}

	return node;
}

Node* Parser::parse_call_param_list(vector<Variable>& signature){
	Node* node = new Node();

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
			if (global_env->get(tmp)) {
				current_env->put(tmp);
				signature.push_back(Variable(tmp->value, tmp->data_type)); 
			}
			else {
				ShowError("TOKEN IS NOT DEFINED: " + tmp->value);
				return false;
			}
		}

		if (current_token->value == ")")
			return node;

		if (!match(new Token(","), false)) {
			return false;
		}
		else
			continue;
	}
}

Node* Parser::parse_var(bool global, bool in_struct, bool new_env, vector<Variable>& signature, bool parse_signature)
{
	Node* variables = new Node();
	variables->data = new Token("var");

	Env* env;

	if (global)
		env = global_env;
	else if(new_env)
		current_env = new Env();

	if (!in_struct) {
		if (!match(new Token("var")))
			return false;
	}
	if (current_token->value == ")")
		return variables;
	else if (current_token->type != Identificator) {
		ShowError("EXPECTED IDENTIFICATOR BUT: " + current_token->type);
		return false;
	}
	
	vector<Token*> tmp_vars;
	bool new_vars = true;
	while (true)
	{
		if (!in_struct) {
			if ((current_token->value == "begin"))
				break;
		}
		else if (parse_signature) {
			if ((current_token->value == ")"))
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
							ShowError("TOKEN ALREADY EXIST: " + current_token->value);
							return false;
						}
					}

					tmp_vars.push_back(current_token);
				}
				else {
					ShowError("TOKEN ALREADY EXIST: " + current_token->value);
					return false;
				}
			}
			else {
				//if (!current_env->get(current_token) && !global_env->get(current_token)) {
				if (!current_env->get(current_token)) {
					for (Token* t : tmp_vars) {
						if (t->value == current_token->value) {
							ShowError("TOKEN ALREADY EXIST: " + current_token->value);
							return false;
						}
					}

					tmp_vars.push_back(current_token);
				}
				else {
					ShowError("TOKEN ALREADY EXIST: " + current_token->value);
					return false;
				}
			}

			if (!match(Identificator)) {
				return false;
			}

			if (!match(new Token(","), false)) {
				Node* tmp = new Node();
				tmp->data = current_token;
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

							if (!parse_signature && !match(new Token(";"))) {
								return false;
							}			
							else {
								for (int i = 0; i < tmp_vars.size(); i++) {

									tmp_vars[i]->data_type = UserDataType;
									tmp_vars[i]->modifier = current_modifier;

									global_env->get(data_type);
									tmp_vars[i]->parent = data_type;

									signature.push_back((Variable(tmp_vars[i]->value, UserDataType)));

									if (global)
										global_env->put(tmp_vars[i]);
									else
										current_env->put(tmp_vars[i]);

								}

								new_vars = true;

								tmp_vars.clear();
								continue;
							}
						}
					}
					else {
						if (parse_signature) {
							if (!match(new Token(";"), false)) {
								if (current_token->value != ")") {
									return false;
								}
							}
						}
						else {
							if (!match(new Token(";"))) {
								return false;
							}
						}

						/*if (!parse_signature && !match(new Token(";"))) { 
							return false;
						}		
						else {*/
							tmp->right = new Node();
							for (int i = 0; i < tmp_vars.size(); i++) {

								if (data_type->value == "integer") {
									tmp->right->data = new Token("integer");
									tmp_vars[i]->data_type = Integer;

									signature.push_back((Variable(tmp_vars[i]->value, Integer)));
								}
								else if (data_type->value == "boolean") {
									tmp->right->data = new Token("boolean");
									tmp_vars[i]->data_type = Boolean;

									signature.push_back((Variable(tmp_vars[i]->value, Boolean)));
								}
								else if (data_type->value == "char") {
									tmp->right->data = new Token("char");
									tmp_vars[i]->data_type = Char;

									signature.push_back((Variable(tmp_vars[i]->value, Char)));
								}
								else if (data_type->value == "double") {
									tmp->right->data = new Token("double");
									tmp_vars[i]->data_type = Double;

									signature.push_back((Variable(tmp_vars[i]->value, Double)));
								}
								else if (data_type->value == "string") {
									tmp->right->data = new Token("string");
									tmp_vars[i]->data_type = String;

									signature.push_back((Variable(tmp_vars[i]->value, String)));
								}

								tmp_vars[i]->modifier = current_modifier;

								if (global)
									global_env->put(tmp_vars[i]);
								else
									current_env->put(tmp_vars[i]);

								Node* t = new Node();
								if (tmp->left != nullptr) {
									t = tmp->left;
									while (t->next != nullptr)
										t = t->next;

									t->next = new Node();
									t->next->data = tmp_vars[i];
								}			
								else {
									tmp->left = new Node();
									tmp->left->data = tmp_vars[i];
								}
									
							}

							Node* t = new Node();
							if (variables->left != nullptr) {
								t = variables->left;
								while (t->next != nullptr)
									t = t->next;

								t->next = tmp;
							}
							else {
								variables->left = new Node();
								variables->left = tmp;
							}							

							new_vars = true;

							tmp_vars.clear();
							continue;
						//}
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

	return variables;
}

Node* Parser::parse_struct()
{
	Node* node = new Node();
	CheckTokenType type;

	struct_env = new Env();

	Token* tmp = current_token;
	current_struct = current_token;
	if (!match(Identificator)) {
		return false;
	}
	else {
		if (global_env->get(tmp)) {
			ShowError("TOKEN ALREADY  EXIST: " + tmp->value);
			return false;
		}
	}

	if (!match(new Token("=")))
		return false;

	node->data = current_token;
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

	/*if (current_token->type == Identificator) {
		if (!parse_var(false, true)) {
			ShowError("BAD VARIABLE  DECLARATION PARSING");
			return false;
		}
	}*/

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
		node->left = parse_var(false, true, true);

		if (!node->left) {
			ShowError("BAD VARIABLE DECLARATION PARSING");
			return false;
		}
	}

	while (current_token->value != "end") 
	{
		Node* tmp = new Node();

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
			tmp = parse_subprogramm(Function, false);
			if (!tmp) {
				ShowError("BAD FUNCTION PARSING");
				return false;
			}
		}
		else if (current_token->value == "procedure") {
			tmp = parse_subprogramm(Procedure, false);
			if (!tmp) {
				ShowError("BAD PROCEDURE PARSING");
				return false;
			}
		}
		else {
			ShowError("EXPECTED FUNCTION OR PROCEDURE DECLARATION BUT: " + current_token->value);
			return false;
		}

		Node* t = new Node();
		if (node->right == nullptr)
			node->right = tmp;
		else {
			t = node->right;
			while (t->next != nullptr)
				t = t->next;

			t->next = tmp;
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

		return node;
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

Node* Parser::stmt()
{
	Node* node = new Node();

	if (current_token == nullptr) {
		return node;
	}

	if (to_lower(current_token->value) == "type") {

		match(new Token("type"));

		while ((current_token->value != "function")
			&& (current_token->value != "procedure")
			&& (current_token->value != "var")
			&& (current_token->value != "begin")) {
			if (!parse_struct()) {
				ShowError("BAD RECORD OR CLASS PARSING");
				return false;
			}
		}
	}
	else if (to_lower(current_token->value) == "function") {
		node = parse_subprogramm(Function);

		if (!node) {
			ShowError("BAD FUNCTION PARSING");
			return false;
		}
	}
	else if(to_lower(current_token->value) == "procedure") {
		node = parse_subprogramm(Procedure);

		if (!node) {
			ShowError("BAD PROCEDURE PARSING");
			return false;
		}
	}
	else if (current_token->value == "begin") {
		operator_brackets_balance++;

		if (!match(new Token("begin")))
			return false;

		node->data = new Token("block");

		node->left = stmt();
		if (!node->left)
			return false;

		if (!match(new Token("end"))) {
				return false;
		}
		else {
			operator_brackets_balance--;
		}		

		pred_token = current_token;
		if (match(new Token(";"), false)) {
			return node;
		}
		else if (current_token->value == "else")
			return node;		
		else if (match(new Token("."))) {
			if (operator_brackets_balance != 0)
				return false;

			if (current_token != nullptr) {
				ShowError("EXPECTED END OF FILE");
				return false;
			}
			else 
				return node;
		}
		else
			return false;
	}
	else if (current_token->value == "end") {
		if (operator_brackets_balance == 0) {
			ShowError("BAD OPERATOR BRACKETS  BALANCE");
			return false;
		}

		return node;
	}
	else if (current_token->value == "var") {
		current_modifier = Public;

		node = parse_var();
		if (!node) {
			ShowError("BAD VARIABLE DECLARATION PARSING");
			return false;
		}
	}
	else if (current_token->value == "if") {
		if (!match(new Token("if")))
			return false;

		node->data = new Token("if");

		if (!match(new Token("("))) 
			return false;

		node->condition = parse_bool_expr();
		if (!node->condition) {
			ShowError("EXPECTED BOOL EXPRESSION");
			return false;
		}

		if (!match(new Token(")")))
			return false;

		if (!match(new Token("then")))
			return false;

		in_block = current_token->value == "begin";

		node->left = stmt();
		if (!node->left) {
			ShowError("EXPECTED EXPRESSION");
			return false;
		}

		if ((match(new Token("else"), false))) {
			if (pred_token->value != ";") 
			{
				in_block = current_token->value == "begin";
				node->right = stmt();
				if (!node->right) {
					ShowError("EXPECTED EXPRESSION");
					return false;
				}
			}
			else {
				ShowError("EXPECTED EXPRESION BUT: ELSE");
				return false;
			}
		}

		node->next = stmt();
		if (!node->next)
			return false;
	}
	else  if (current_token->type == Identificator) {
		save_state();

		if (!global_env->get(current_token) && !current_env->get(current_token)) {
			ShowError("TOKEN NOT DEFINED: " + current_token->value);
			return false;
		}

		if (current_token->check_type == Var) {
			node = parse_expr();
			if (!node) {
				ShowError("EXPECTED EXPRESSION");
				return false;
			}
		}
		else if ((current_token->check_type == Function) ||
				(current_token->check_type == Procedure)) {
			if (!parse_call(current_token))
				load_state();
		}	

		if (!match(new Token(";"), false)) {
			if (current_token->value == "else")
				return node;
			else
				return false;
		}
		else {
			if (in_block)
				node->next = stmt();
			else
				return node;
			
			if (!node->next) {
				ShowError("EXPECTED EXPRESSION");
				return false;
			}
		}
			
	}
	else if (current_token->value == "while") {
		node->data = current_token;

		if (!match(new Token("while")))
			return false;

		if (!match(new Token("(")))
			return false;
		
		node->condition = parse_bool_expr();
		if (!node->condition) {
			ShowError("EXPECTED BOOL EXPRESSION");
			return false;
		}

		if (!match(new Token(")")))
			return false;

		if (!match(new Token("do")))
			return false;

		in_block = current_token->value == "begin";
		
		node->left = stmt();

		if (!node->left) {
			ShowError("EXPECTED EXPRESSION");
			return false;
		}

		node->next = stmt();
		if (!node->next)
			return false;
	}
	else {
		ShowError("ERROR  TOKEN: " + current_token->value);
		return false;
	}

	return node;
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
	pred_token = new Token();
}

Parser::~Parser(){}