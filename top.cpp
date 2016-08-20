/*******************************************************************************
 * Name        : top.cpp
 * Author      : Ben Blease
 * Email       : bblease@stevens.edu
 * Date        : 6/12/16
 * Description : Main File for one2one Script
 ******************************************************************************/

#include "interp.h"
#include "parser.h"
#include <cstring>
#include <fstream>

using namespace std;

/*
*Parse the file, and initialize the interpreter
*/
Interpreter * initialize(string file){
	Parser *parser = new Parser();
	vector<string> scanned = parser->scan(file);
	vector<Token> token_vec = parser->tokenize(scanned);
	parser->check_parens(token_vec);
	Program *program = parser->create_program(token_vec);
	Interpreter *interp = new Interpreter(program);
	delete parser;
	return interp;
}

int main(int argc, char* argv[]){
	if (argc != 1){
		cout << "Usage: ./one2one" << endl;
	}

	cout << "one2one Script v 1.0 | c. 2016 Ben Blease | bblease@stevens.edu" << endl;

	while (true){
		std::cout << "Run: ";
		string file_name;
		cin >> file_name;

		if (file_name.compare("q") == 0){
			break;
		}

		else if (file_name.compare("readme") == 0){
			ifstream readme("./readme.md");
			string rm = "\033[1;36m" + string(istreambuf_iterator<char>(readme), istreambuf_iterator<char>()) + "\033[0m";
			cout << rm << endl;
		}

		else if (file_name.compare("grammar") == 0){
			ifstream grammar("./grammar.txt");
			string g = "\033[1;36m" + string(istreambuf_iterator<char>(grammar), istreambuf_iterator<char>()) + "\033[0m";
			cout << g << endl;
		}

		else {
			ifstream input_file(file_name);

			if (!input_file){
				cerr << "Error: Cannot open file '" << file_name << "'." << endl;
			}
			else {
				input_file.exceptions(ifstream::badbit);

				string master = string(istreambuf_iterator<char>(input_file), istreambuf_iterator<char>());

				Interpreter *main;
				//catch any parsing errors
				try {
					main = initialize(master);
				} catch (ParseException &e) {
					main = NULL;
					cerr << e.parsing_error() << endl;
					return 1;
				}
				//catch any interpreting errors
				try {
					main->start();
				} catch (InterpreterException &e) {
					cerr << e.interp_error() << endl;
					return 1;
				}

				delete main;
			}
		}
	}
	return 0;
}
