/*******************************************************************************
 * Name        : environment.h
 * Author      : Ben Blease
 * Date        : 6/12/16
 * Description : Environment Structures for one2one Script
 ******************************************************************************/
#include "environment.h"

struct ExpVal {
	virtual void accept(Visitor &v) = 0;

	virtual int get_bc() = 0;
};

//variable environment
//as it stands, all variables are global in scope TODO
class Environment{
public:
	Environment() { }

	~Environment() { }

	/*
	*Adds a variable to the environment
	*/
	inline void extend_env(char var, int val){
		env_[var] = val;
	}

	/*
	*Yields the reference index within the environment for provided var
	*/
	inline unsigned int apply_env(char var){
		return env_[var];
	}

	/*
	*Yields a boolean determining whether a char is already held within the env
	*/
	bool in_env(char var){
		std::map<char, int>::iterator it;
		it = env_.find(var);
		return it != env_.end();
	}

	void print(){
		std::map<char, int>::iterator it;
		for (it = env_.begin(); it != env_.end(); ++it){
			std::cout << it->first << " " << it->second << std::endl;
		}
		std::cout << std::endl;
	}

private:
	std::map<char, int> env_;
};

//variable store
class Store{
public:
	Store() { }

	~Store() { }

	/*
	*Returns the current length of the store
	*/
	inline unsigned int get_length(){
		return store_.size();
	}

	/*
	*Adds variable value to the store
	*/
	inline void extend_store(ExpVal* val){
		store_.push_back(val);
	}

	/*
	*Adds variable value to the store
	*Used if variable is already present
	*/
	void extend_store(unsigned int ref, ExpVal* val){
		if (ref >= store_.size()){
			extend_store(val);
		}else{
			store_[ref] = val;
		}
	}

	/*
	*Returns the value to a variable in the store
	*/
	inline ExpVal * apply_store(unsigned int i){
		return store_[i];
	}

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

inline ExpVal * create_val(int x){
	return new NumVal(x);
}

inline ExpVal * create_val(bool x){
	return new BoolVal(x);
}
