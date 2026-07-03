//This is the main entry point of the function

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

int main(int argCount, char*argVector[]) {    //this is the main fuction, int means that it will return an int value to whatever ran it, 0 means success and 1 means failure
//arg count is the number of arguments going in, argVector is the actual user input function
// use is going to change to include a calctype option at the beginning
// 0 will be the binary being actually called, 1 will be calctype, 2 will be the messy expression, then 3-5 will be the options for graphing

    if (config::debugMode) { //if debug mode print the arg vector we started the main with
        std::cout <<"Main started with:";
        for (int n = 0; n < argCount; n++){
            std::cout << argVector[n] << " ";
        }
        std::cout  << " \n";
    }

    double solution; //initializes solution as a double

    if (argCount < 2) {
        std::cerr << "error: No expression provided. Usage: ./engine [calcType] [expr] or ./engine [calcType] [expr] [minX] [maxX] [xCount]" << "\n";
        return 1; //exit safely instead of trying to read empty arguments
    }

    std::string calcType = argVector[1]; //defines and stores the calctype option
        /*s will be used to say its a scientific calculation, g will be graphing. in the future c will be cas, d will be derivative, i will be integral.*/

    std::string messyFunction = argVector[2]; //defines the messy expression as the first argument for the input passed by the api

    //safely preload variables for when not graphing
    double xMin = 0.0;
    double xMax = 0.0;
    double xCount = 0.0;

    //run the function through the cleaner, lexer, and parser regardless of if we are graphing or not
    std::string cleanFunction = cleaner(messyFunction); //declares a string variable to hold a cleaned up input and gets the cleanFunction by passing the messy function to the cleaner
    std::vector < std::string > tokenizedFunction = lexer(cleanFunction); //declares a vector of string variables to hold the tokens for the input and gets the tokens by passing the function to the lexer
    std::vector < std::string > parsedFunction= parserast(tokenizedFunction); //declares a vector of string variables to hold a cleaned up input and parses the function by passing the tokens to the parser

    // we want to check if the function contained an x variable
    bool containsX = false; //initializes the bool

    //checks if the string contains an x
    if (containsString("x", parsedFunction) == true){
        containsX = true; //if it does then it sets the variable to true
    }

    if (containsString("X", parsedFunction) == true){
            parsedFunction = replaceStrings(parsedFunction, "X", "x"); //if x is upper case it replaces it with a lower case x
            containsX = true; //then sets containsX to true
        }

    //if calcType is s
    if (calcType == "s"){
        if (containsX){ // and contains an x
            parsedFunction = replaceStrings(parsedFunction, "x", "0"); //replace the x in the parsedFunction with 0
            solution = interpreter(parsedFunction); //sends the parsedFunction to the interpreter to be solved for the y intercept
        }
        else if (!containsX){ //and if it doesn't contain x then just pass it to the evaluator
            solution = interpreter(parsedFunction);
        }

        std::cout << solution << " \n"; //prints the solution to the console

        if (!config::debugMode){ //if we aren't debugging we can just exit after this
            return 0;
        }
    }

    //if we are doing a graphing calculation then we can calculate x,y values, to start I will make it generate a vector of values within our xMin and xMax, then calculate the solution for each x value
    else if (calcType == "g") {
        
        if (containsX){
            //load the actual variables if they exist
            xMin = std::stod(argVector[3]); //states the xMin is the second input
            xMax = std::stod(argVector[4]); //states the xMax is the third input
            xCount = std::stod(argVector[5]); //states the xCount is the fourth input

            std::vector < std::pair < double, double >> xyPairs; //initializes a vector of pairs to hold xy values

            xyPairs = graphpoints(parsedFunction, xMin, xMax, xCount); //passes the parsedFunction into the graphpoints tool

            for (int n = 0; n < xyPairs.size(); n++ ) { //for every pair in the xyPairs vector
                std::cout << "(" << xyPairs[n].first << ", " << xyPairs[n].second << ")" << " \n"; //prints each pair as (xValue, yValue)
            }

            std::vector < std::pair < double, double >> yInt; //initializes a vector of pairs to hold the yInt
            yInt = graphpoints(parsedFunction, 0.0, 1.0, 1.0);
            solution = yInt[0].second;
        }

        if (!config::debugMode){ //if we aren't debugging we can just exit after this
            return 0;
        }
    }

    else { //if it does not fall into the above categories then it is not going to work
        std::cerr << "error: This calculation is not supported: Failure in main.cpp" << " /n";
        for (int n = 0; n < argCount; n++) { //this loop will print an error I can see in the network response page while inspecting payloads and responses
            std::cerr << argVector[n] << " ";
        }
        std::cerr << "/n";
        return 1; //exit safely instead of trying to read empty arguments
    }

    if (config::debugMode) { //prints the messy function and clean function if debug mode is true
        std::cout << "Main finished successfully with:" << " /n";
        std::cout << messyFunction << "    Cleaned to:    " << cleanFunction << " /n";
        std::cout << "Tokenized to:  "; //prints each token inside brackets for our tokenized vector
        for (const std::string& token : tokenizedFunction) {
            std::cout << "[" << token << "], ";
        }
        std::cout << std::endl;
        std::cout << "Parsed to:  "; //prints each token inside brackets for the parsed vector
        for (const std::string& token : parsedFunction) {
            std::cout << "[" << token << "], ";
        }
        std::cout << std::endl;
        std::cout << "solution is: "<< solution << " /n";
    }
    
    return 0;
}
