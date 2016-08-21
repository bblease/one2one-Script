/*******************************************************************************
 * Name        : environment.h
 * Author      : Ben Blease
 * Date        : 6/12/16
 * Description : Environment Structures for one2one Script
 ******************************************************************************/

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "expression.h"
#include <map>

struct ExpVal {
	virtual void accept(Visitor &v) = 0;

	virtual int get_bc() = 0;
};

//variable environment
//as it stands, all variables are global in scope TODO
class Environment{
public:
	Environment();

	~Environment();

	void extend_env(char, int);

	size_t apply_env(char);

	bool in_env(char);

	void print();

private:
	std::map<char, int> env_;
};

//variable store
class Store{
public:
	Store();

	~Store();

	size_t get_length();

	void extend_store(ExpVal*);

	void extend_store(size_t, ExpVal*);

	ExpVal * apply_store(size_t);

private:
	std::vector<ExpVal*> store_;
};

//Expressed values are Closures, Numbers, and Booleans
//Values are encapsulated to allow for Closures to be stored within the store.

enum {
	CLOSURE,
	NUMVAL,
	BOOLVAL
};

struct Closure : ExpVal {
	char var;
	Expression *body;
	Environment *env;

	void accept(Visitor &v){
		v.visit(*this);
	}

	int get_bc(){
		return CLOSURE;
	}

	explicit Closure(char var, Expression *body, Environment *env): var(var), body(body), env(env) { };

	explicit Closure(Expression *body, Environment *env): body(body), env(env) { };
};

struct NumVal : ExpVal {
	int num;

	void accept(Visitor &v){
		v.visit(*this);
	}

	int get_bc(){
		return NUMVAL;
	}

	explicit NumVal(int num): num(num) {}
};

struct BoolVal : ExpVal {
	bool val;

	void accept(Visitor &v){
		v.visit(*this);
	}

	int get_bc(){
		return BOOLVAL;
	}

	explicit BoolVal(bool val): val(val) {}
};

ExpVal * create_val(int);

ExpVal * create_val(bool);

#endif /* ENVIRONMENT_H_ */
