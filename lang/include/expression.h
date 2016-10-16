/*******************************************************************************
 * Name        : expression.h
 * Author      : Ben Blease
 * Email       : bblease@stevens.edu
 * Date        : 6/12/16
 * Description : Expression Structures for one2one Abstract Syntax Tree
 ******************************************************************************/

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "visit_interface.h"
#include <iostream>
#include <vector>
#include <stack>

enum Id {
    NULLEXP,
    BLOCKEXP,
    NUMEXP,
    OPEXP,
    BOOLEXP,
    BOOLOP,
    NUMBOOLEXP,
    VAREXP,
    SETEXP,
    IFEXP,
    FOREXP,
    WHILEXP,
    PROCEXP,
    CALLEXP,
    PRINTEXP,
    INCEXP,
    DELEXP,
    EMPTYEXP
};

struct Expression {
  virtual void accept(Visitor &v) = 0;

  //returns the specific type of an expression
  virtual Id get_bc() = 0;

  virtual ~Expression() { }
};

struct Program {
  Expression *exp;

  Program(Expression *exp): exp(exp) { }

  ~Program() {
    delete exp;
  }
};

struct NullExp : Expression {
  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return NULLEXP;
  }

  NullExp() { }

  ~NullExp() { }
};

struct BlockExp : Expression {
  std::vector<Expression*> exps;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return BLOCKEXP;
  }

  BlockExp(std::vector<Expression*> exps): exps(exps) { }

  ~BlockExp(){
    for (std::vector<Expression*>::iterator i = exps.begin(); i != exps.end(); ++i){
      delete (*i);
    }
  }

};


struct NumExp : Expression {
  int integer;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return NUMEXP;
  }

  NumExp(const int integer): integer(integer){ }

  NumExp() { }

  ~NumExp() { }
};


struct OpExp : NumExp {
  Expression *arg1;
  Expression *arg2;
  char op;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return OPEXP;
  }

  OpExp(Expression *arg1,
                Expression *arg2,
                char op):
                  NumExp(),
                  arg1(arg1),
                  arg2(arg2),
                  op(op) {}

  ~OpExp() {
    delete arg1;
    delete arg2;
  }
};

struct BoolExp : Expression {
  bool arg;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return BOOLEXP;
  }

  BoolExp(bool arg): arg(arg) { }

  BoolExp() { }

  ~BoolExp() { }
};

//typical boolean operations
struct BoolOp : BoolExp {
  Expression *arg1 = NULL;
  Expression *arg2 = NULL;
  char op;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return BOOLOP;
  }

  BoolOp(Expression *arg1,
                  Expression *arg2,
                  char op):
                  BoolExp(),
                  arg1(arg1),
                  arg2(arg2),
                  op(op) {}

  //constructor for not
  BoolOp(Expression *arg1,
                  char op):
                  BoolExp(),
                  arg1(arg1),
                  op(op) { }

  ~BoolOp() {
    delete arg1;
    delete arg2;
  }
};

struct NumBoolExp : BoolExp {
  Expression *arg1;
  Expression *arg2;
  char op;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return NUMBOOLEXP;
  }

  NumBoolExp(Expression *arg1,
                      Expression *arg2,
                      char op):
                      BoolExp(),
                      arg1(arg1),
                      arg2(arg2),
                      op(op) { }

  ~NumBoolExp() {
    delete arg1;
    delete arg2;
  }
};

struct VarExp : Expression {
  char var;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return VAREXP;
  }

  VarExp(char var): var(var){ }

  ~VarExp() { }
};

struct SetExp : Expression {
  char var;
  Expression *expr;
  //Type type;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return SETEXP;
  }

  SetExp(char var,
                  Expression *expr):
                  var(var),
                  expr(expr) { }

  ~SetExp() {
    delete expr;
  }
};

struct IfExp : Expression{
  Expression *exp1 = NULL; //if
  Expression *exp3 = NULL; //if

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return IFEXP;
  }

  IfExp(Expression *exp1,
                Expression *exp3):
                exp1(exp1),
                exp3(exp3) { }

  ~IfExp() {
    delete exp1;
    delete exp3;
  }
};

struct ForExp : Expression {
  char it; //iterating variable
  Expression *lo;
  Expression *hi;
  Expression *exp1;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return FOREXP;
  }

  ForExp(char it,
                  Expression *lo,
                  Expression *hi,
                  Expression *exp1):
                  it(it),
                  lo(lo),
                  hi(hi),
                  exp1(exp1) { }

  ~ForExp() {
    delete lo;
    delete hi;
    delete exp1;
  }
};

struct WhileExp : Expression {
  Expression *exp1;
  Expression *exp2;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return WHILEXP;
  }

  WhileExp(Expression *exp1,
                    Expression *exp2):
                    exp1(exp1),
                    exp2(exp2) { }

  ~WhileExp() {
    delete exp1;
    delete exp2;
  }
};

struct ProcExp : Expression {
  char name;
  char inputs = '\0';
  Expression *body;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return PROCEXP;
  }

  ProcExp(char name,
                  char inputs,
                  Expression *body):
                  name(name),
                  inputs(inputs),
                  body(body) { }

  ProcExp(char name,
                  Expression *body):
                  name(name),
                  body(body) { }

  ~ProcExp() {
    delete body;
  }
};

struct CallExp: Expression{
  char name;
  Expression *arg = NULL;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return CALLEXP;
  }

  CallExp(char name, Expression *arg): name(name), arg(arg) { }

  CallExp(char name): name(name) { }

  ~CallExp() {
    delete arg;
  }
};

struct PrintExp : Expression {
  Expression *prnt;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return PRINTEXP;
  }

  PrintExp(Expression *prnt): prnt(prnt) { }

  ~PrintExp() {
    delete prnt;
  }
};

struct IncExp : Expression {
  char var;
  Expression *exp;

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc(){
    return INCEXP;
  }

  IncExp(char var, Expression *exp): var(var), exp(exp) { }

  ~IncExp() {
    delete exp;
  }
};

struct DelExp : Expression {

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc() {
    return DELEXP;
  }

  DelExp() { }

  ~DelExp() { }
};

struct EmptyExp : Expression {

  inline void accept(Visitor &v){
    v.visit(*this);
  }

  inline Id get_bc() {
    return EMPTYEXP;
  }

  EmptyExp() { }

  ~EmptyExp() { }
};


#endif /* EXPRESSION_H_ */
