#ifndef INTERP_H_
#define INTERP_H_
#include "error_handling.h"
#include "environment.h"
#include <stack>
#include <algorithm>

//visit_inferface.h
class NodeVisitor : public Visitor {
public:
	NodeVisitor(std::stack<int> &value_stack,
				std::stack<Expression*> &exp_stack,
				std::stack<Closure> &closure_stack):
				value_stack_(value_stack),
				exp_stack_(exp_stack),
				closure_stack_(closure_stack) { }

	~NodeVisitor() { }

	/*
	*Moves all AST members to the stacks for interpreting
	*/
	virtual void visit(NullExp &exp) { }

	virtual void visit(BlockExp &exp){
		for (int i = exp.exps.size()-1; i >= 0; i--){
			exp_stack_.push(exp.exps[i]);
		}
	}

	virtual void visit(NumExp &exp){
		value_stack_.push(exp.integer);
	}

	virtual void visit(OpExp &exp){
		//add expressions to std::stack first
		exp_stack_.push(exp.arg2);
		exp_stack_.push(exp.arg1);
		value_stack_.push(exp.op);
	}

	virtual void visit(BoolExp &exp){
		value_stack_.push(exp.arg);
	}

	virtual void visit(BoolOp &exp){
		if (exp.arg2 != NULL){
			exp_stack_.push(exp.arg2);
		} else {
			exp_stack_.push(new NullExp);
		}
		exp_stack_.push(exp.arg1);
		value_stack_.push(exp.op);
	}

	virtual void visit(NumBoolExp &exp){
		exp_stack_.push(exp.arg2);
		exp_stack_.push(exp.arg1);
		value_stack_.push(exp.op);
	}

	virtual void visit(VarExp &exp){
		value_stack_.push(exp.var);
	}

	virtual void visit(SetExp &exp){
		value_stack_.push(exp.var);
		exp_stack_.push(exp.expr);
	}

	virtual void visit(IfExp &exp){
		exp_stack_.push(exp.exp3);
		exp_stack_.push(exp.exp1);
	}

	virtual void visit(ForExp &exp){
		value_stack_.push(exp.it);
		exp_stack_.push(exp.exp1);
		exp_stack_.push(exp.hi);
		exp_stack_.push(exp.lo);
	}

	virtual void visit(WhileExp &exp){
		exp_stack_.push(exp.exp2);
		exp_stack_.push(exp.exp1);
	}

	virtual void visit(ProcExp &exp){
		value_stack_.push(exp.inputs);
		value_stack_.push(exp.name);
		exp_stack_.push(exp.body);
	}

	virtual void visit(CallExp &exp){
		if (exp.arg != NULL){
			exp_stack_.push(exp.arg);
		} else {
			//tell the interpreter that the expression is concluded on the exp_stack_
			exp_stack_.push(new NullExp());
		}
		value_stack_.push(exp.name);

	}

	virtual void visit(PrintExp &exp){
		exp_stack_.push(exp.prnt);
	}

	virtual void visit(IncExp &exp){
		value_stack_.push(exp.var);
		exp_stack_.push(exp.exp);
	}

	virtual void visit(DelExp &exp) { }

	virtual void visit(EmptyExp &exp) { }

	/*
	*Move all expressed values onto stacks for interpreting
	*/
	virtual void visit(Closure &exp){
		closure_stack_.push(exp);
	}

	virtual void visit(NumVal &exp){
		value_stack_.push(exp.num);
	}

	virtual void visit(BoolVal &exp){
		value_stack_.push(exp.val);
	}

private:
	std::stack<int> &value_stack_;
	std::stack<Expression*> &exp_stack_;
	std::stack<Closure> &closure_stack_;
};

/*
*Main interpreter for the language
*/
class Interpreter {
public:
	Interpreter(Program*);

	~Interpreter();

	template <class T>
	T stack_get(std::stack<T>&);

	void interpret(Expression*);

	void start();

private:
	Program *program_;
	Environment *env_ = new Environment();
	Store *store_ = new Store();
	std::stack<int> value_stack_; //value storage for interpreter
	std::stack<Expression*> exp_stack_; //stores pointers for processing
	std::stack<Closure> closure_stack_; //stores closures for processing by CALLEXPs
	NodeVisitor visitor_ = NodeVisitor(value_stack_, exp_stack_, closure_stack_);
};

#endif /* INTERP_H_ */
