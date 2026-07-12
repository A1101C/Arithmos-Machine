#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string> //because lexer will work with std::string
#include <vector> //imports the vector

//this says that the parser function will take a vector of strings, and return a double
double interpreter(std::vector < std::string > inputFunction); 

double evaluator(std::vector<std::string> orderedVector); //this will let me call the evaluator in the graphpoints tool

#endif