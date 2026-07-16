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


// arguments will be passed like calcType{messyFunction}
// and if you know the xmin max and count you can pass it as calcType{messyFunction} xMin xMax xCount

int main(int argCount, char*argVector[]) {    //this is the main fuction, int means that it will return an int value to whatever ran it, 0 means success and 1 means failure

    if (argCount > 2){
    //this grabs the last two positions in the inputVector
        for (int n = argCount - 1; n >= (int)argCount - 2; n--) {
            std::string tempFlag = argVector[n];  //set a tempFlag equal to the inputVector position n
            if (tempFlag == "-d"){
                config::debugMode = true; //if either flag is -d set debugMode to true
            }
            if (tempFlag == "-s"){
                config::showSteps = true; //if either flag is -s set show steps to true
            }
            else;
                continue;
        }
    }

    if (config::debugMode) { //if debug mode print the arg vector we started the main with
        std::cout <<"Main started with:";
        for (int n = 0; n < argCount; n++){
            std::cout << argVector[n] << " ";
        }
        std::cout  << " \n";
    }

    if (argCount < 2) { //makes sure we have the bare minimum number of arguments to run
        std::cerr << "error: Usage: ./engine \"calcType(expr)\" flags\n";
        std::cerr << "Valid calcType: graph(expr), sci(expr)\n";
        std::cerr << "Valid Flags: -d and or -e\n";
        std::cerr << "Valid Flags: for graphing xMin xMax xCount or xMin xMax xCount and -d and or -e\n";
        
        return 1;
    }

    double solution; //initializes solution as a double

    std::string rawString = argVector[1]; //saves the raw input in the vector position 1 as rawString
    std::string calcType; //initializes a string to hold the calctype
    std::string messyFunction; //initializes a string to hold the messyFunction
    int exprStart; //makes an integer to keep track of the starting position for the expression
    int exprEnd = rawString.length(); //makes an integer to keep track of the end position for the expression

    //Input expression Parser start
    //the purpose of this is to turn calcType(expression) into calcType and expression but leaving any parenthesis inside of expression untouched
    for ( int n = 0; n < rawString.length(); n++){ //for the length of the rawString
        if (rawString[n] != '('){ //loop through looking at the nth position making sure its not '(' and just continuing until you find '('
            continue;
        }
        else //if it finds '(' save its position as n, and exit the loop
            exprStart = n;
            break; 
    }

    calcType = rawString.substr(0, exprStart); //make calcType equal to the substring we extract from rawString starting at position 0 and pulling out exprStart number of positions
    messyFunction = rawString.substr(exprStart+1, exprEnd-exprStart-2); //see above, the +1 makes us start at the position past the open parenthesis and the -2 and the end ensure we dont get the closed parenthesis
    //Input expression Parser end

    if (config::debugMode){
                std::cout << "Input Parser ended with: " << calcType << " " << messyFunction << " \n";
            } 


    //run the function through the cleaner, lexer, and parser regardless of if we are graphing or not
    std::string cleanFunction = cleaner(messyFunction); //declares a string variable to hold a cleaned up input and gets the cleanFunction by passing the messy function to the cleaner
    std::vector < std::string > tokenizedFunction = lexer(cleanFunction); //declares a vector of string variables to hold the tokens for the input and gets the tokens by passing the function to the lexer
    std::vector < std::string > parsedFunction= parserast(tokenizedFunction); //declares a vector of string variables to hold a cleaned up input and parses the function by passing the tokens to the parser

    // we want to check if the function contained an x variable
    bool containsX = false; //initializes the bool

    //checks if the string contains an x
    if (containsString("x", parsedFunction) || containsString("X", parsedFunction) ){ //if the string contains either x or upper case X
        parsedFunction = replaceStrings(parsedFunction, "X", "x"); //if X is upper case it replaces it with a lower case x
        containsX = true; //then sets containsX to true
    }

    int calcInt = 0;
    if (calcType == "sci"){
        calcInt = 1;
    }
    if (calcType == "graph"){
        calcInt = 2;
    }

    switch (calcInt) { //switch statement is faster than a bunch of if else statements
        // the item in the above parenthesis is compared to each case below and that is how it selects the right option
        case 1:
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
        
            break;

        case 2:
            if (containsX){
            //preload the x range information to defaults
            double xMin = -100;
            double xMax = 100;
            double xCount = 200;

                //load the actual variables if they exist

                if (argCount >= 5) { //if there are enough input arguments try to turn them into doubles
                    try {
                        xMin = std::stod(argVector[2]);
                        xMax = std::stod(argVector[3]);
                        xCount = std::stod(argVector[4]);
                    } 
                    catch (const std::exception& e) {
                        //if stod fails then use defaults, might fail if its a letter for some reason
                        std::cerr << "Warning: Invalid bounds provided. Using defaults.\n";
                        xMin = -100.0;
                        xMax = 100.0;
                        xCount = 200.0;
                    }
                } 
                
                else {
                    //if argCount is less than five they didnt enter enough arguments so just proceed with the defaults we preloaded
                    if (config::debugMode) {
                        std::cout << "Optional bounds missing, using default.\n";
                    }
                }
                
                std::vector < std::pair < double, double >> xyPairs; //initializes a vector of pairs to hold xy values

                xyPairs = graphpoints(parsedFunction, xMin, xMax, xCount); //passes the parsedFunction and x range information into the graphpoints tool

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
            break;

        default:
            std::cerr << "error: This calculation is not supported: Failure in main.cpp" << " /n";
            for (int n = 0; n < argCount; n++) { //this loop will print an error I can see in the network response page while inspecting payloads and responses
                std::cerr << argVector[n] << " ";
            }
            std::cerr << "/n";
            return 1; //exit safely instead of trying to read empty arguments
            
            break;
    }

    if (config::debugMode) { //prints the messy function and clean function if debug mode is true
        std::cout << "Main finished successfully with:" << " \n";
        std::cout << messyFunction << "    Cleaned to:    " << cleanFunction << " \n";
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
        std::cout << "Solved to: "<< solution << " \n";
    }
    
    return 0;
}
