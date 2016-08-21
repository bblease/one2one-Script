/******************************************************************************
 * Name        : error_handling.cpp
 * Author      : Ben Blease
 * Email       : bblease@stevens.edu
 * Date        : 8/10/16
 * Description : Error handling for one2one Script
 ******************************************************************************/
#include "error_handling.h"

const char* NameException::parsing_error() const throw() {
 return "\033[1;31mSyntax Error :\033[0m No name given for function, variable, run.";
}

const char* ParenException::parsing_error() const throw() {
 return "\033[1;31mSyntax Error :\033[0m A closing parentheses, bracket, or end statement is missing.";
}

const char* ExtraParen::parsing_error() const throw() {
 return "\033[1;31mSyntax Error :\033[0m An extra closing parentheses, bracket, or end statement is present.";
}

const char* TokenException::parsing_error() const throw() {
 return "\033[1;31mSyntax Error :\033[0m Unknown token.";
}

const char* SyntaxException::parsing_error() const throw() {
 return "\033[1;31mSyntax Error :\033[0m There's an error in your syntax.";
}

const char* StackError::interp_error() const throw() {
 return "\033[1;31mStack Error :\033[0m missing expected number or expression.";
}
