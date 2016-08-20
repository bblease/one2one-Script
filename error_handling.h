/******************************************************************************
 * Name        : error_handling.h
 * Author      : Ben Blease
 * Email       : bblease@stevens.edu
 * Date        : 8/10/16
 * Description : Error handling for one2one Script
 ******************************************************************************/

#ifndef ERROR_HANDLING_H_
#define ERROR_HANDLING_H_
#include <exception>

 class ParseException : public std::exception {
 public:
 	virtual const char* parsing_error() const throw() = 0;
 };

 class NameException : public ParseException {
 public:
 	virtual const char* parsing_error() const throw() { 
 		return "\033[1;31mSyntax Error :\033[0m No name given for function, variable, run.";
 	}
 };

 class ParenException : public ParseException {
 public:
 	virtual const char* parsing_error() const throw() { 
 		return "\033[1;31mSyntax Error :\033[0m A closing parentheses, bracket, or end statement is missing.";
 	}
 };

  class ExtraParen : public ParseException {
 public:
 	virtual const char* parsing_error() const throw() { 
 		return "\033[1;31mSyntax Error :\033[0m An extra closing parentheses, bracket, or end statement is present.";
 	}
 };

 class TokenException : public ParseException {
 public:
 	virtual const char* parsing_error() const throw() {
 		return "\033[1;31mSyntax Error :\033[0m Unknown token.";
 	}
 };

 class SyntaxException : public ParseException {
 	public:
 	virtual const char* parsing_error() const throw() {
 		return "\033[1;31mSyntax Error :\033[0m There's an error in your syntax.";
 	}
 };

 class InterpreterException: public std::exception {
 public:
 	virtual const char* interp_error() const throw() = 0;
 };

 class StackError: public InterpreterException {
 public:
 	virtual const char* interp_error() const throw() { 
 		return "\033[1;31mStack Error :\033[0m missing expected number or expression.";
 	}
 };


 #endif /* ERROR_HANDLING_H_ */