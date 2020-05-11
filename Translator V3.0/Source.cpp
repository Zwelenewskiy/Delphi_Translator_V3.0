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

void show_mixed(Node* root, int depth)//функция для вывода дерева методом обхода в ширину
{
	if (root->data[0]->value == ":=")
	{
		show_mixed(root->left, depth);
	}
	else if (root->data[0]->value == "block") {

		cout << root->data[0]->value << " " << endl;
		show_mixed(root->next, depth);
	}
	else {
		if (root->left != nullptr) {
			show_mixed(root->left, depth + 2);
		}

		for (int i = 0; i < depth; i++) {
			cout << "    ";
		}

		cout << root->data[0]->value << " " << endl;

		if (root->condition != nullptr) {
			show_mixed(root->condition, depth + 1);
		}

		if (root->right != nullptr) {
			show_mixed(root->right, depth + 2);
		}


		if (depth == 0) {
			cout << endl;
		}
	}
}

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