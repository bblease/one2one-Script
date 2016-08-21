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

struct ExpVal;

class Environment;

class Store;

struct Closure;

struct NumVal;

struct BoolVal;

ExpVal * create_val(int x);

ExpVal * create_val(bool x);

#endif /* ENVIRONMENT_H_ */
