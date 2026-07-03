//this is the lexer, used to parse the cleaned string into a series of single token strings

#include <iostream> //required for any basic input/output operations
#include <cctype> //needed for isalpha
#include <string> //imports the string library
#include <vector> //imports the vector
#include "config.h" //includes the config file containing debug variables
#include "utils.h" //this has the containsString function


//Begining of the lexer function
std::vector < std::string > lexer(std::string inputFunction) {

    if (config::debugMode) { //prints the inputFunction for the lexer to the console if debug is true
        std::cout << "Lexer Started Successfully" <<std::endl;
    }

    std::vector < std::string > lexedTokens; //initializes the vector to hold the tokens

    std::string currentToken; //creates a string to hold the token currently being assembled

    std::vector < std::string > operators = {"(", ")", "^", "*", "/", "+", "-", "="}; //makes a vector to hold operators
    std::vector < std::string > trigFunctions = {"sin", "cos", "tan", "sec", "csc", "cot"}; //a vector holding trig functions
    std::vector < std::string > logFunctions = {"log", "ln"}; //a vector to hold log, ln, and e. 

    for (int n = 0; n < inputFunction.length(); ) { //for every position in the input function
        char currentChar = inputFunction[n]; //the current character is in the n position

        if (std::isdigit(currentChar) || currentChar == '.') { //if currentChar is a digit or a decimal place
            currentToken.push_back(currentChar); //add it to the current token
            while (n + 1 < inputFunction.length() && (std::isdigit(inputFunction[n + 1]) || inputFunction[n + 1] == '.')){ //and as long as the next char exists and is also a number
                n++; //this moves n to the next position if it exists and is a digit
                currentToken.push_back(inputFunction[n]); //add it to the end of the current token
            }
            lexedTokens.push_back(currentToken); //add the entire token we just made to the vector
            currentToken.clear(); //now clear the token so that we can collect another one
            n++; //advance to the next position in the inputFunction
        }

        //now find out if the next character is alpha
        else if (std::isalpha(currentChar)) {
        
            std::string nextThree; //creates a string to hold the next 3 characters

            for (int i = n; i < n + 3 && i < inputFunction.length(); i++ ) { /*for i equal to n while i is less than n+3 and i is less than the input length
                the reason for that is because we want the next 3 parts of the string including our current n and the next two but not the third, hence the less than
                rather than a less than or equal to. we also want to make sure we are not trying to read a position that does not exist, like n when n is the last
                part of the string and i+1 would be a non existant space*/
                nextThree.push_back(inputFunction[i]); //adds the next three characters to the nextThree string
            }

            bool foundTrig = false; //because I wrapped the if statement for finding trig functions in a for loop I need a way to tell the next else if to skip
            bool foundLog = false; //because I wrapped the if statement for finding log functions in a for loop I need a way to tell the next else if to skip

            for (const std::string& trig : trigFunctions) {   //check if the length of the string is long enough to be a trig function
                if (n + trig.length() <= inputFunction.length() && inputFunction.substr(n, trig.length()) == trig) { //makes sure input Function is long enough to have a trig function, and loops through the trig function vector to determine if one exists in the inputFunction and assigns it to trig
                    lexedTokens.push_back(trig); //put the trig substring in the lexedTokens vector

                    n += trig.length(); //skips n to go past the length of the trig function
                    foundTrig = true; //found trig is true
                    break; //immediately leave the if and for loops
                }
            }

            if (foundTrig) { 
                continue; //skip back to the top
            } 

            for (const std::string& log : logFunctions) {
                if (n + log.length() <= inputFunction.length() && inputFunction.substr(n, log.length()) == log) {//makes sure input Function is long enough to have a log, and loops through the log function vector to determine if one exists in the inputFunction and assigns it to log
                    lexedTokens.push_back(log); // add the matched log token

                    n += log.length();  //dynamically advance past its length, 3 for log, 2 for ln
                    foundLog = true;  //mark that we mfound a log
                    break;
                }
            }

            if (foundLog) {
                continue; // jump back to the top
            }  

            else if (currentChar == 'x' || currentChar == 'X' || currentChar == 'e' ) { //if the next token is an x or e
                if (currentChar == 'X'){
                    currentChar = 'x'; //if x is upper case replace it with lower case
                }

                char lastChar(inputFunction[n - 1]); //declares a pointer called lastChar and makes it hold the value of the previous item in the inputFunction
                
                if (std::isdigit(lastChar)) { //if the last character was a digit
                    lexedTokens.push_back("*"); //add * to the token vector before the x or e
                }
                
                currentToken.push_back(currentChar); //add it to the currentChar string
                lexedTokens.push_back(currentToken); //add the currentToken to the token vectors

                if (n + 1 < inputFunction.length()) { //if the next position exists
                    std::string nextChar(1, inputFunction[n + 1]); //make it equal to the string nextChar, the (1,inputFunction) is to say that the char we are pulling from input function is the 1st character in the string nextChar
                    if (!containsString(nextChar, operators)) { //if the next character is not an operator
                        lexedTokens.push_back("*"); //put * between x and it.
                    }
                }
                currentToken.clear(); //clear the current token
                n++; //advance to the next position in the inputFunction
            }

            else { //if the next token us an unknown alpha print an error saying what it is and skip
                std::cout << "Lexer Error: Unhandled variable: " << currentChar <<std::endl;
                n++; //continues to the next n if the letter was unknown
            }
        }

        else if (containsString(std::string(1, currentChar), operators)) { //converts the currentChar into a string so I can compare it against operators, and checks if the next token is an operator
            currentToken.push_back(currentChar); //add it to the currentChar string
                lexedTokens.push_back(currentToken); //add the currentToken to the token vector
                currentToken.clear(); //clears the currentToken
                n++; //advance to the next position in the inputFunction
        }
    
        else {
            std::cout << "Lexer Error: Unknown String in n = " << n << " position." << std::endl; //if we run into an unknown string print its position
            n++; //advance by one so we can continue evaluating the function
        }
    }

    if (config::debugMode) { //successfully finished debug message
        std::cout << "Lexer Finished Successfully" <<std::endl;
    }

    return lexedTokens;
}