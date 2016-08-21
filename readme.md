#One2one Script v 1.0
c. 2016 Ben Blease
bblease@stevens.edu

A simple interpreted functional programming language written in c++.
The aim of one2one is to provide an easy to learn language thats capable of
running mathematical functions.

#Planned Improvements

Lists and strings support.
More descriptive error handling.
A typed version.

#Usage

Bash is preferable for usage, whether in Linux or Windows. Cygwin also works.
The makefile is included, run make to build the program.
Run ./one2one, then specify the .txt file you'd like to run.
The grammar for this language is also included.
Type "grammar" to review it within the command line.
Type "readme" to review this readme in the command line.
Type "q" to exit the interpreter.
"#" denotes inline comments
$ denotes block comments $

#Limitiations

Tail recursion has yet to be implemented.
Currently, numbers are stored as integers, so no floats are supported.
All variables and function names are stored as an alphabetical char for easy
storage.
There are no else if/else expressions available.
No strings or lists are available. However, lists can be implemented within the
language. Check arrays.txt for examples.
All variables are global in scope.
If two run expressions are operated on, they MUST be enclosed in ( ), otherwise the parser won't be able to correctly build the expressions.

Please report any bugs you find by email!
