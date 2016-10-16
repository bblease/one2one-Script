/******************************************************************************
 * Name        : parser.h
 * Author      : Ben Blease
 * Email       : bblease@stevens.edu
 * Date        : 8/1/16
 * Description : Header for parsing.
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

	Token(tok);

	Token(tok, char);

	Token(tok, int);

	Token(tok, bool);

	~Token();
};

class Parser {
public:
	Parser();

	~Parser();

	std::vector<std::string> scan(std::string);

	bool is_keyword(std::string);

	bool is_ignored(std::string);

	void correct(std::vector<Token>&);

	std::vector<Token> tokenize(std::vector<std::string>);

	void check_parens(std::vector<Token>);

	void eat(std::vector<Token>&);

	void eat(tok t, std::vector<Token>&);

	Expression * parse(std::vector<Token>&);

	Program * create_program(std::string);

private:
	//pair RESERVED WORDS with tokens
	std::map<std::string, tok> toks_ = {
		{"if", if_tok},
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
