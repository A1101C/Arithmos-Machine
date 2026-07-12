#ifndef GRAPHPOINTS_H
#define GRAPHPOINTS_H

#include <iostream> //required for any basic input/output operations
#include <cctype> //needed for isalpha
#include <string> //imports the string library
#include <vector> //imports the vector
#include "config.h" //includes the config file containing debug variables
#include "utils.h" //this has the containsString function

#include "interpreter.h" // includes my interpreter

//pass the parsed function and graphing arguments into the function
std::vector < std::pair < double, double >> graphpoints(std::vector < std::string> parsedFunction, 
                                                        double xMin, 
                                                        double xMax, 
                                                        double xCount); 

#endif