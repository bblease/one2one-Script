#ifndef ERROR_HANDLING_H_
#define ERROR_HANDLING_H_
#include <exception>

class ParseException : public std::exception {
public:
 virtual const char* parsing_error() const throw() = 0;
};

class NameException : public ParseException {
public:
 virtual const char* parsing_error() const throw();
};

class ParenException : public ParseException {
public:
 virtual const char* parsing_error() const throw();
};

 class ExtraParen : public ParseException {
public:
 virtual const char* parsing_error() const throw();
};

class TokenException : public ParseException {
public:
 virtual const char* parsing_error() const throw();
};

class SyntaxException : public ParseException {
 public:
 virtual const char* parsing_error() const throw();
};

class InterpreterException: public std::exception {
public:
 virtual const char* interp_error() const throw() = 0;
};

class StackError: public InterpreterException {
public:
 virtual const char* interp_error() const throw();
};

class VariableError: public InterpreterException {
public:
  virtual const char* interp_error() const throw();
};

 #endif /* ERROR_HANDLING_H_ */
