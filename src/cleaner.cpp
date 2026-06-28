//this is to clean an input into a standard format for my lexer to parse

#include <iostream> //required for any basic input/output operations
#include <string> //imports the string library
#include <cctype> //needed for isalpha
#include "config.h" //includes the config file containing debug variables
#include "cleaner.h" //includes the cleaner.h file so it knows what to do
#include "utils.h" //this has the containsString and replaceAll function

//beginning of the cleaner function
std::string cleaner(std::string inputFunction){

    std::string outputFunction; //initializes a string to hold the cleaned outputFunction

    if (config::debugMode) { //prints the inputFunction for the cleaner to the console if debug is true
        std::cout << "Cleaner Started Successfully" <<std::endl;
    }

    replaceAll(inputFunction, " ", "");

    if (inputFunction == "x"){
        inputFunction = "1x";
    }

/*    for (int n = 0; n < inputFunction.length() - 1; ++ n) { //creates int variable n and sets it equal to 0, and as long as n is less than the length of the inputFunction string -1 run the stuff run the loop and add 1 to n at the end
        if (std::isdigit(inputFunction[n]) && (inputFunction[n+1]) == 'x') //if the character in position n is a digit and the character in position n+1 is x run next, if not it does ++n
            inputFunction.insert(n+1, "*"); //if above is true it inserts * into the n+1 position, not replaces
        if (std::isalpha(inputFunction[n]) && (inputFunction[n+1]) == '(') //if the character in position n is alphanumeric and the character in position n+1 is ( run next, if not it does ++n
            inputFunction.insert(n+1, "*"); //if above is true it inserts * into the n+1 position, not replaces
        if ((inputFunction[n]) == ')' && std::isalpha(inputFunction[n+1]) && ) //if the character in position n is a closed parenthesis and the character in position n+1 is alphanumeric run next, if not it does ++n
            inputFunction.insert(n+1, "*"); //if above is true it inserts * into the n+1 position, not replaces
    }*/ //commented out because I moved this functionality to the lexer

    outputFunction = inputFunction;

    if (config::debugMode) { //prints the outputFunction for the cleaner to the console if debug is true
        std::cout << "Cleaner Finished Successfully" <<std::endl;
    }

    return outputFunction; // returns the cleaned function out of the cleaner
}