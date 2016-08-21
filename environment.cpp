/*******************************************************************************
 * Name        : environment.h
 * Author      : Ben Blease
 * Date        : 6/12/16
 * Description : Environment Structures for one2one Script
 ******************************************************************************/
#include "environment.h"

Environment::Environment() { }

Environment::~Environment() { }

/*
*Adds a variable to the environment
*/
void Environment::extend_env(char var, int val){
	env_[var] = val;
}

/*
*Yields the reference index within the environment for provided var
*/
unsigned int Environment::apply_env(char var){
	return env_[var];
}

/*
*Yields a boolean determining whether a char is already held within the env
*/
bool Environment::in_env(char var){
	std::map<char, int>::iterator it;
	it = env_.find(var);
	return it != env_.end();
}

void Environment::print(){
	std::map<char, int>::iterator it;
	for (it = env_.begin(); it != env_.end(); ++it){
		std::cout << it->first << " " << it->second << std::endl;
	}
	std::cout << std::endl;
}

Store::Store() { }

Store::~Store() { }

/*
*Returns the current length of the store
*/
unsigned int Store::get_length(){
	return store_.size();
}

/*
*Adds variable value to the store
*/
void Store::extend_store(ExpVal* val){
	store_.push_back(val);
}

/*
*Adds variable value to the store
*Used if variable is already present
*/
void Store::extend_store(unsigned int ref, ExpVal* val){
	if (ref >= store_.size()){
		extend_store(val);
	}else{
		store_[ref] = val;
	}
}

/*
*Returns the value to a variable in the store
*/
ExpVal * Store::apply_store(unsigned int i){
	return store_[i];
}


ExpVal * create_val(int x){
	return new NumVal(x);
}

ExpVal * create_val(bool x){
	return new BoolVal(x);
}
