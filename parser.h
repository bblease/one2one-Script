/******************************************************************************
 * Name        : parser.h
 * Author      : Ben Blease
 * Email       : bblease@stevens.edu
 * Date        : 8/1/16
 * Description : Parsing for one2one using a top down recursive parser
 ******************************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

#include "error_handling.h"
#include "expression.h"
#include <string>
#include <algorithm>
#include <sstream>
#include <map>

enum tok {
	if_tok, else_if_tok, else_tok,//if
	for_tok, while_tok, //loops
	int_tok, bool_tok,//values
	func, name_tok, assign_tok, run_tok, print_tok, inc_tok, //function
	del_tok, top_tok, empty_tok,//stack
	leftp, rightp,//( )
	num_ops, check_lg, check_equal, //numeric operations
	and_tok, or_tok, xor_tok, not_tok,//boolean operations
	begin_tok, end_tok//stack exps
};

struct Token {
	tok typ;
	char val = '\0';
	int num = 0;
	bool bol = 0;

	Token(tok  typ): typ(typ) {}

	Token(tok typ, char val): typ(typ), val(val) { }

	Token(tok typ, int num): typ(typ), num(num) { }

	Token(tok typ, bool bol): typ(typ), bol(bol) { }

	~Token() { }
};

class Parser {
public:
	Parser() { }
 
	~Parser() { }

	/*
	*Simply splits std::strings based on delimiting characters
	*/
	std::vector<std::string> scan(std::string s){
		std::vector<std::string> out;
		bool comment = false;
		std::string focus;
		for (unsigned int i = 0; i < s.length(); i++){
			//check if comment
			if (s[i] == '#' || s[i] == '$'){
				comment = true;
			}
			//check if char is delimeter
			else if (!comment && (
				s[i] == ' ' ||
				s[i] == '+' ||
				s[i] == '-' ||
				s[i] == '*' ||
				s[i] == '/' ||
				s[i] == '%' ||
				s[i] == '^' ||
				s[i] == '<' ||
				s[i] == '>' ||
				s[i] == '.' ||
				s[i] == '(' ||
				s[i] == ')' ||
				s[i] == '{' ||
				s[i] == '}' ||
				s[i] == '\t' ||
				s[i] == '\n' || 
				s[i] == '\r')){
				//add std::string to output
				if (focus != ""){
					out.push_back(focus);
				}
				if (s[i] == '+' && s[i+1] == '+'){
					focus = "++";
					out.push_back(focus);
					i += 2;
				}
				//only add to output if the char is an operator
				if (s[i] != ' ' && s[i] != '\n' && s[i] != '\r' && s[i] != '\t'){
					//convert to std::string
					std::string a = std::string(1, s[i]);
					out.push_back(a);
				}
				focus = "";
			}else if (i == s.length()-1){
				//if the end is reached, add what's left
				focus += s[i];
				out.push_back(focus);
			}else if (!comment && s[i]) {
				focus += tolower(s[i]);	
			}else if (i == s.length()-1 || s[i] == '\n' || s[i] == '$'){
				//comments end at the end of a line or the end of the file
				comment = false;
			}
		}
		return out;
	}

	/*
	*Check if string is a keyword
	*/
	bool is_keyword(std::string var){
		std::map<std::string, tok>::iterator it;
		it = toks_.find(var);
		return it != toks_.end();
	}

	/*
	*Check if string is ignored
	*/
	bool is_ignored(std::string a){
		return std::find(std::begin(ignored_), std::end(ignored_), a) != std::end(ignored_);
	}

	/*
	*Corrects tokenized input operators
	*May need correction
	*/
	void correct(std::vector<Token> &t){
		for (unsigned int i = 0; i < t.size(); i++){
			if (t[i].typ == assign_tok ||
				t[i].typ == num_ops ||
				t[i].typ == and_tok ||
				t[i].typ == or_tok ||
				t[i].typ == xor_tok ||
				t[i].typ == check_equal ||
				t[i].typ == check_lg){
				if (t[i-1].typ == rightp){ 
					std::stack<char> open; //determine if expression is correctly traversed
					for (unsigned int k = i; k >= 0; k--){
						if (t[k].typ == rightp){
							open.push('$');
						}
						else if (t[k].typ == leftp){
							open.pop();
						}
						if (open.empty() && k != i){
							t.insert(t.begin()+k, t[i]); //move operator
							t.erase(t.begin()+i+1); //delete operator from former position	
							break;
						}
					}
				} else {
					Token swap = t[i];
					t[i] = t[i-1];
					t[i-1] = swap; 
				}
			}
		}
	}

	/*
	*Tokenizes delimited std::string resulting from scan
	*May need revisions, currently inefficient
	*/
	std::vector<Token> tokenize(std::vector<std::string> s){
		std::vector<Token> out;
		for (unsigned int i = 0; i < s.size(); i++){
			int num;
			bool bol;
			//iss for ints iss2 for bools
			std::istringstream iss(s[i]);
			std::istringstream iss2(s[i]);
			if (is_keyword(s[i])){
				//create token
				Token tk = Token(toks_[s[i]]);
				out.push_back(tk);
			} 
			//check if the string is a name
			else if (s[i].length() == 1 && isalpha(s[i][0])) {
				out.push_back(Token(name_tok, s[i][0]));
			}
			//check if the string is an integer
			else if (iss >> num){
				out.push_back(Token(int_tok, num));
			} 
			//check if the string is a boolean
			else if (iss2 >> std::boolalpha >> bol){
				out.push_back(Token(bool_tok, bol));
			} 
			//check if the string is an operator
			else if (s[i].length() == 1 && !isalpha(s[i][0])) {
				if (s[i][0] == '+' ||
					s[i][0] == '-' ||
					s[i][0] == '*' ||
				    s[i][0] == '/' ||
					s[i][0] == '%' ||
					s[i][0] == '^'){
					out.push_back(Token(num_ops, s[i][0]));
				}
				else if (s[i][0] == '<' ||
						 s[i][0] == '>'){
					out.push_back(Token(check_lg, s[i][0]));
				}
			}
			//throw error if token can't be created
			else if (!is_ignored(s[i])){
				std::cout << s[i] << std::endl;
				throw TokenException();
			}
		}
		correct(out);
		return out;
	}

	/*
	*Check for missing paretheses, brackets, and end statements
	*/
	void check_parens(std::vector<Token> p){
		std::stack<char> check;
		for (size_t i = 0; i < p.size(); i++){
			if (p[i].typ == begin_tok || p[i].typ == leftp){
				check.push('$');
			} 
			else if (p[i].typ == end_tok || p[i].typ == rightp){
				if (!check.empty()){
					check.pop();
				} else {
					throw ExtraParen();
				}
			}
		}
		if (!check.empty()) {
			throw ParenException();
		}
	}

	/*
	*Eats the head of token vector
	*/
	inline void eat(std::vector<Token> &p){
		p.erase(p.begin());
	}

	/*
	*Eats the current token if it matches the specified type
	*/
	void eat(tok t, std::vector<Token> &p){
		if (p[0].typ != t){
			throw SyntaxException();
		} else {
			eat(p);
		}
	}

	/*
	*Create abstract syntax tree from tokens
	*/
	Expression * parse(std::vector<Token> &p){
		Expression *out;
		tok t = p[0].typ;
		//load information from token
		int num = p[0].num;
		char val = p[0].val;
		bool bol = p[0].bol;
		eat(p); //eat current token
		switch(t){
			default:
				out = new NullExp();
				break;

			case leftp: {
				out = parse(p);
				eat(rightp, p);
				break;
			}
			
			//build block
			case begin_tok: {
				std::vector<Expression*> exps; 
				while (p[0].typ != end_tok){
					Expression *exp = parse(p);
					exps.push_back(exp);
				}
				//tell the interpreter that the expression is concluded on the PTR_STACK
				exps.push_back(new NullExp()); 
				out = new BlockExp(exps);
				eat(end_tok, p);
				break;
			}

			case int_tok:  {
				out = new NumExp(num);
				break;
			}

			case num_ops: {
				out = new OpExp(parse(p), parse(p), val);
				break;
			}

			case bool_tok: {
				out = new BoolExp(bol);
				break;
			}

			case not_tok: {
				out = new BoolOp(parse(p), '!');
				break;
			}
			case and_tok: 
			case or_tok:
			case xor_tok: {
				out = new BoolOp(parse(p), parse(p), ops_[t]);
				break;
			}

			case check_equal: {
				out = new NumBoolExp(parse(p), parse(p), '=');
				break;
			}

			case check_lg: {
				out = new NumBoolExp(parse(p), parse(p), val);
				break;
			}

			case name_tok: {
				out = new VarExp(val);
				break;
			}

			case assign_tok: {
				if (p[0].typ == name_tok){
					char name = p[0].val;
					eat(name_tok, p); //eat name
					out = new SetExp(name, parse(p));
				} else {
					throw NameException();
				}
				break;
			}

			case if_tok: {
				out = new IfExp(parse(p), parse(p));
				break;
			}

			case for_tok: {
				if (p[0].typ == name_tok){
					char it = p[0].val;
					eat(name_tok, p);
					out = new ForExp(it, parse(p), parse(p), parse(p));
				} else {
					throw NameException();
				}
				break;	
			}

			case while_tok: {
				out = new WhileExp(parse(p), parse(p));
				break;
			}

			case func: {
				if (p[0].typ == name_tok){
					char name;
					char var;
					name = p[0].val;
					eat(name_tok, p);
					//make sure variables are encapsulated in ( )
					if (p[0].typ == leftp){
						eat(leftp, p);
						//make sure a variable is present
						if (p[0].typ == name_tok){
							var = p[0].val;
							eat(name_tok, p);
							eat(rightp, p); 
						} else {
							eat(rightp, p);
							var = '\0';
						}
					} else {
						var = '\0';
					}
					if (var == '\0'){
						out = new ProcExp(name, parse(p));
					} else {
						out = new ProcExp(name, var, parse(p));
					}
				} else {
					throw NameException();
				}
				break;
			}

			case run_tok: {
				
				if (p[0].typ == name_tok){
					char name = p[0].val;
					eat(p);
					if (p[1].typ == rightp){
						eat(leftp, p);
						eat(rightp, p);
						out = new CallExp(name);
					} else {
						eat(leftp, p);
						out = new CallExp(name, parse(p));
						eat(rightp, p);
					}
				} else {
					throw NameException();
				}

				break;
			}

			case print_tok: {
				out = new PrintExp(parse(p));
				break;
			}

			case inc_tok: {
				if (p[0].typ == name_tok){
					out = new IncExp(p[0].val, 	parse(p));
				} else {
					throw NameException();
				}
				break;
			}

			case del_tok: {
				out = new DelExp();
				break;
			}

			case top_tok: {
				out = new NullExp();
				break;
			}

			case empty_tok: {
				out = new EmptyExp();
				break;
			}
		}
		return out;
	}

	/*
	*Create the final program for interpretation
	*Also check if all parentheses are closed.
	*/
	Program * create_program(std::vector<Token> p){
		Program *out = new Program(parse(p));
		return out;
	}

private:
	//pair RESERVED WORDS with tokens
	std::map<std::string, tok> toks_ = {
		{"if", if_tok},
		{"elif", else_if_tok},
		{"else", else_tok},
		{"for", for_tok},
		{"while", while_tok},
		{"function", func},
		{"=", assign_tok},
		{"run", run_tok},
		{"print", print_tok},
		{"++", inc_tok},
		{"(", leftp},
		{")", rightp},
		{"and", and_tok},
		{"or", or_tok},
		{"xor", xor_tok},
		{"not", not_tok},
		{"equals?", check_equal},
		{"begin", begin_tok},
		{"end", end_tok},
		{"{", begin_tok},
		{"}", end_tok},
		{"++", inc_tok},
		{"del", del_tok},
		{"top", top_tok},
		{"empty", empty_tok}
	};

	//ignored strings
	std::string ignored_[3] = {"each", "from", "to"};

	//token type to char pairing
	std::map<tok, char> ops_ = {
		{and_tok, '&'},
		{or_tok, '|'},
		{xor_tok, '^'}
	};
};
	
#endif /* PARSER_H_ */