/*******************************************************************************
* Name        : interp.h
* Author      : Ben Blease
* Email       : bblease@stevens.edu
* Date        : 6/12/16
* Description : Interpreter for one2one Script
******************************************************************************/
#include "interp.h"

Interpreter::Interpreter(Program *program):
			program_(program) { }

Interpreter::~Interpreter() {
	delete program_;
	delete env_;
	delete store_;
	//if expression stack isn't empty, clear.
	if (!exp_stack_.empty()){
		while (!exp_stack_.empty()){
			exp_stack_.pop();
		}
	}
}

/*
*Returns and removes top element of stack
*/
template <class T>
T Interpreter::stack_get(std::stack<T> &s){
	if (s.empty()){
		throw StackError();
	}
	T out = s.top();
	s.pop();
	return out;
}

/*
*Interprets a given expression
*The interpreter receives inputs from stack and pushes results back on stack
*Usage : interpret(program->expression)
*Function interprets all expressions in program recursively
*Interpreter receives necessary information from above stacks
*/
void Interpreter::interpret(Expression *exp){
	int bc = exp->get_bc(); //retrieve bytecode from abstract syntax tree node
	exp->accept(visitor_);
	switch(bc){
		case NULLEXP: {
			break;
		}

		case BLOCKEXP: {
			 //load members to stacks
			while(exp_stack_.top()->get_bc() != NULLEXP){
				Expression *a = stack_get<Expression*>(exp_stack_);
				interpret(a);
			}
			exp_stack_.pop(); //remove the NullExp from stack
			break;
		}

		case NUMEXP: {
			break;
		}

		case OPEXP: {
			//interpret each operand
			interpret(stack_get<Expression*>(exp_stack_));
			interpret(stack_get<Expression*>(exp_stack_));
			//load operands and operator
			int b = stack_get<int>(value_stack_);
			int a = stack_get<int>(value_stack_);
			char c = stack_get<int>(value_stack_);
			//conduct operation based on operator
			switch(c){
				case '+':
					value_stack_.push(a + b);
					break;
				case '-':
					value_stack_.push(a - b);
					break;
				case '*':
					value_stack_.push(a * b);
					break;
				case '/':
					value_stack_.push(a / b);
					break;
				case '%':
					value_stack_.push(a % b);
					break;
				case '^':
					value_stack_.push(pow(a, b));
					break;
			}
			break;
		}

		case BOOLEXP: {
			break;
		}

		case BOOLOP: {
			interpret(stack_get<Expression*>(exp_stack_));
			int b = stack_get<int>(value_stack_);
			int a = false;
			//stack may be empty if the operator is "!"
			if (exp_stack_.top()->get_bc() != NULLEXP){
				interpret(stack_get<Expression*>(exp_stack_));
				a = stack_get<int>(value_stack_);
			} else {
				exp_stack_.pop();
			}

			//load operator
			char c = stack_get<int>(value_stack_);

			switch(c){
				case '&':
					value_stack_.push(a && b);
					break;
				case '|':
					value_stack_.push(a || b);
					break;
				case '^':
					value_stack_.push(a ^ b);
					break;
				case '!':
					value_stack_.push(!b);
					break;
			}
			break;
		}

		case NUMBOOLEXP: {
			interpret(stack_get<Expression*>(exp_stack_));
			interpret(stack_get<Expression*>(exp_stack_));
			//load operator and operands
			int b = stack_get<int>(value_stack_);
			int a = stack_get<int>(value_stack_);
			char c = stack_get<int>(value_stack_);
			switch(c){
				case '=':
					value_stack_.push(a == b);
					break;
				case '<':
					value_stack_.push(a < b);
					break;
				case '>':
					value_stack_.push(a > b);
					break;
			}
			break;
		}

		case VAREXP: {
			char a = stack_get<int>(value_stack_);
			//if the variable is already in the environment, use
			if (env_->in_env(a)){
				unsigned int b = env_->apply_env(a);
				ExpVal* c = store_->apply_store(b);
				c->accept(visitor_);
			} else {
				//else add to environment and store with place holders
				env_->extend_env(a, store_->get_length());
				store_->extend_store(NULL);
			}
			break;
		}

		case SETEXP: {
			//load from stack into function
			char var = stack_get<int>(value_stack_);
			Expression* val = stack_get<Expression*>(exp_stack_);
			interpret(val);
			ExpVal* new_val = create_val(stack_get<int>(value_stack_));
			//if the variable is already in the environment, update
			if (env_->in_env(var)){
				store_->extend_store(env_->apply_env(var), new_val);
			//otherwise, add to store and environment
			}else{
				env_->extend_env(var, store_->get_length());
				store_->extend_store(new_val);
			}
			break;
		}

		case IFEXP: {
			interpret(stack_get<Expression*>(exp_stack_));
			//run if
			if(stack_get<int>(value_stack_)){
				interpret(stack_get<Expression*>(exp_stack_));
			}
			else{
				exp_stack_.pop();
			}
			break;
		}

		case FOREXP: {
			//determine the lower and upper bounds
			interpret(stack_get<Expression*>(exp_stack_));
			interpret(stack_get<Expression*>(exp_stack_));
			//load final bounds
			int hi = stack_get<int>(value_stack_);
			int lo = stack_get<int>(value_stack_);
			//retrieve iterator
			char it = stack_get<int>(value_stack_);

			//add iterator to the environment
			if (!env_->in_env(it)){
				env_->extend_env(it, store_->get_length());
			}

			//load expression within for loop
			Expression* a = stack_get<Expression*>(exp_stack_);
			for (int i = lo; i < hi; i++) {
				ExpVal *curr_it = create_val(i);
				store_->extend_store(env_->apply_env(it), curr_it);
				interpret(a);
			}
			break;
		}

		case WHILEXP: {
			Expression *a = stack_get<Expression*>(exp_stack_);
			Expression *b = stack_get<Expression*>(exp_stack_);
			interpret(a);
			while(stack_get<int>(value_stack_)){
				interpret(b);
				interpret(a);
			}
			break;
		}

		//manufacture a closure and add to store and environment
		case PROCEXP: {

			//load all needed values
			Expression *body = stack_get<Expression*>(exp_stack_);
			char name = stack_get<int>(value_stack_);
			Closure *c;
			if (value_stack_.top() != (int) '\0'){

				char var = stack_get<int>(value_stack_);
				env_->extend_env(var, store_->get_length());
				store_->extend_store(NULL);
				c = new Closure(var, body, env_);
			} else {
				value_stack_.pop(); //remove null char
				c = new Closure(body, env_);
			}
			//add closure to store and environment
			env_->extend_env(name, store_->get_length());
			store_->extend_store(c);
			break;
		}

		case CALLEXP: {
			//load all parameters from procedure onto stack
			char name = stack_get<int>(value_stack_);
			ExpVal *c = store_->apply_store(env_->apply_env(name));

			//after receiving closure, reload to stacks
			//needed to access closure members
			//closure NOT a pointer
			c->accept(visitor_);
			Closure clos = stack_get<Closure>(closure_stack_);
			//NullExp denotes the ending of an expression
			if (exp_stack_.top()->get_bc() != NULLEXP){
				Expression *arg = stack_get<Expression*>(exp_stack_);
				interpret(arg);
				store_->extend_store(env_->apply_env(clos.var), create_val(stack_get<int>(value_stack_)));
			} else {
				exp_stack_.pop(); //be sure to remove the NullExp so it isn't interpreted
			}

			interpret(clos.body);

			break;
		}

		case PRINTEXP: {
			interpret(stack_get<Expression*>(exp_stack_));
			std::cout << stack_get<int>(value_stack_) << std::endl;
			break;
		}

		case INCEXP: {
			char var = stack_get<int>(value_stack_);
			interpret(stack_get<Expression*>(exp_stack_));
			int a = stack_get<int>(value_stack_) + 1;
			store_->extend_store(env_->apply_env(var), create_val(a));
			break;
		}

		case DELEXP: {
			if (!value_stack_.empty()){
				value_stack_.pop();
			}
			break;
		}

		case EMPTYEXP: {
			value_stack_.push(value_stack_.empty());
			break;
		}
	}
}

/*
*Initiates interpreter, checks for errors at the end of runtime
*/
void Interpreter::start(){
	Expression *expr = program_->exp;
	interpret(expr);
}
