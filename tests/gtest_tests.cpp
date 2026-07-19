//Write tests in here for each part of the calculator

#include <gtest/gtest.h>

#include <iostream> //required for any basic input/output operations
#include <cctype> //needed for isalpha
#include <string> //imports the string library
#include <vector> //imports the vector
#include <cstdlib>   //imports atof to convert a string to a double

#include "config.h" //includes the config file containing debug variables
#include "cleaner.h" //includes the cleaner.h which defines the cleaner function in cleaner.cpp
#include "lexer.h" //includes the lexer.h file defining the lexer
#include "parserast.h" //includes the parser and ast file
#include "interpreter.h" //includes the interpreter.h file defining the interpreter
#include "graphpoints.h" //includes the graphing function


//sanity check to make sure the most basic of math works in the evaluator
TEST(interpreterTestManual, basicAddition) {
    std::vector < std::string > tokens = {"1", "+", "1", "=", "T1"}; //the interpreter takes a vector of strings like this
    double solution = interpreter(tokens); //the interpreter returns the solution as a double
    EXPECT_DOUBLE_EQ(solution, 2.0); //this says we expect the result to be a double and that we are making sure that solution is a double equal to 2.0
}