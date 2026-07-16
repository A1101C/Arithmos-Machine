//this is the parser-ast, used to parse the tokens and organize them into a tree to be evaluated

#include <iostream> //required for any basic input/output operations
#include <cctype> //needed for isalpha
#include <string> //imports the string library
#include <vector> //imports the vector
#include <cctype> //used for isalnum
#include "config.h" //includes the config file containing debug variables
#include "utils.h" //this has the containsString and replaceAll function

//start of a basic attempt to organize the tokens based on PEMDAS
std::vector < std::string > pemdas(std::vector<std::string> inputVector, int operatorCount, int& opCounter) {
    std::vector < std::string > orderedVector; //creates the vector where the correctly ordered functions and their calculations will be kept.
    std::vector < std::string > operators = {"(", ")", "^", "*", "/", "+", "-", "="}; //makes a vector to hold operators
    std::vector < std::string > trigFunctions = {"sin", "cos", "tan", "sec", "csc", "cot"}; //vector to hold trig functions
    std::vector < std::string > logFunctions = {"log", "ln"}; //vector to hold log functions
    std::vector < std::string > subExpression; //a temporary vector to hold the subexpressions inside of parenthesis
    std::vector < std::string > subResults; //a temporary vector to hold the subexpressions reduced form

    while (containsString("(", inputVector)) { //while the input vector has an open parenthesis
        int openParenthesis = -1; //initializes a variable to hold the position of our inner most open parenthesis
        int closedParenthesis = -1; //initializes a variable to hold the position of the inner most closed parenthesis
        for (int n = inputVector.size() - 1; 0 <= n; n-- ){ //starting from the end of the vector go to the left to find the inner most set of open parenthesis
            if (inputVector[n] == "("){ //when you find an open parenthesis in position n
                openParenthesis = n; //set the variable open parenthesis equal to it
                break; //then immediately leave the if statement
            }
        }
        for (int n = openParenthesis; n < inputVector.size(); n++){ //now starting from the open parenthesis and moving to the right find the first closed parenthesis
            if (inputVector[n] == ")"){ //if the n position in the input vector is )
                closedParenthesis = n;
                break; //leave the if statement                  
            }
        }
        subExpression.clear(); //makes sure the sub expression is empty
        subExpression.assign(
            inputVector.begin() + openParenthesis + 1, // the vector.function(); functions uses the range as [start, stop) so it includes the first option but excludes the last one
            inputVector.begin() + closedParenthesis
        ); // the above 3 lines assign all the strings from the input vector between the positions of openParenthesis + 1, to closedParenthesis to the subExpression. this ensures we dont bring the parenthesis with us

        std::string tempToken; //declare the tempToken string

        int operatorCount_subExpression = operatorCounter(subExpression); //this calls the operator count function from the utils
        subResults = pemdas(subExpression, operatorCount_subExpression, opCounter); 

        if (!subResults.empty()) { //makes sure that the subexpression is not empty
            tempToken = subResults.back(); //grabs the last token generated
        }
        else { //if subResults is empty it means there were no operators inside the parenthesis so its just a number like 4 or something
            tempToken = subExpression[0]; //sets our placeholder token directly to that single number string inside the parenthesis
        }

        //now we need to delete the strings from the input vector that we have pulled out
        //std::string tempToken = "T" + std::to_string(opCounter); //makes a temporary token based on the number of operators we have parsed through
        //opCounter ++; //increments the opCounter

        inputVector.erase( //erase the sub expression from the inputVector
            inputVector.begin() + openParenthesis, // the vector.function(); functions uses the range as [start, stop) so it includes the first option but excludes the last one
            inputVector.begin() + closedParenthesis + 1 // + 1 ensures we erase the closed parenthesis too
        );

        inputVector.insert(inputVector.begin() + openParenthesis, tempToken); // inserts a place holder where the open parenthesis used to be, this also shifts any strings at or to the right of inputVector[openParenthesis] to the right by one
    
        orderedVector.insert(orderedVector.end(), subResults.begin(), subResults.end()); //insert, at the end of ordered vector, everything from subresults beginning to the subresults end
    }

    std::vector < std::vector < std::string > > operatorPriority = { //this is a vector of vectors, the inner vectors contain equal priority operators
        {"sin", "cos", "tan", "sec", "csc", "cot", "log", "ln"}, //Tier 0
        {"^"},          //Tier 1 
        {"*", "/"},     //Tier 2
        {"+", "-"},     //Tier 3
        {"="},          //Tier 4
    };

    //now insert items into the orderedVector from the inputVector by the order of operations 
    std::vector < std::string > currentPriority; //initializes a vector to hold the currentPriority tier
    int tieBreaker = 0; //this is to determine the winning index if we have both * and /
    for ( int n = 0; n < operatorPriority.size(); n++ ) { //this will process through 0-3 for the 3 tiers in the operator priority vector of vectors
        currentPriority = operatorPriority[n]; // sets the current priority vector equal to our the vector tier n from operator Priority
        while(containsAny(currentPriority, inputVector)){ //while the input vector contains any strings from the current priority list
            for (int n = 0; n < inputVector.size(); n++ ) { //look left to right for the current priority token
                if (containsString(inputVector[n], currentPriority)) { //if the current string from the input vector is in the current priority tier
                    tieBreaker = n; //the tie breaker is equal to the nth position
                    break; //immediately stop because we found the left most operator
                }
            }

            //this is the reduction part to remove from input function and put in orderedVector in the right order
            std::string op = inputVector[tieBreaker]; //sets our tokens from the inputvector to other temporary tokens based on their position in relation to the operator
            std::string leftValue = "0"; //left value is 0 by default and only if we are not evaluating a trig function will it assign a value to it
            std::string rightValue = inputVector[tieBreaker + 1];
            
            bool hasLeftValue = false; //creates a true false bool incase we are dealing with a trig function that has no value to the left


            if (tieBreaker > 0 && !containsString(op, trigFunctions) && !containsString(op, logFunctions)) { //if the operator is not a trig function or log function
                leftValue = inputVector[tieBreaker - 1]; //grab the actual number/token on the left
                hasLeftValue = true; //mark that we consumed a left token
            }

            std::string tempToken = "T" + std::to_string(opCounter); //makes a temporary token based on the number of operators we have parsed through
            opCounter++;        

            orderedVector.push_back(leftValue); //add the value to the left to the end of the ordered vector
            orderedVector.push_back(op); //add the operator to the end of the ordered vector
            orderedVector.push_back(rightValue); //add the value to the right to the end of the ordered vector
            orderedVector.push_back("="); //add an equal to sign to show that the left is equal to our temp token
            orderedVector.push_back(tempToken); //add the placeholder value that the expression is equal to

            if (hasLeftValue) {
                inputVector.erase(inputVector.begin() + tieBreaker - 1, inputVector.begin() + tieBreaker + 2);  //erase the three components left, op, and right
                inputVector.insert(inputVector.begin() + tieBreaker - 1, tempToken); //put the temporary token exactly where the left value used to be in the input function
            }
            else {
                inputVector.erase(inputVector.begin() + tieBreaker, inputVector.begin() + tieBreaker + 2); //now only erases 2 tokens from the vector pool because it had a trig function
                inputVector.insert(inputVector.begin() + tieBreaker, tempToken); 
            }

            /*the above will take an input vector like:
            [3], [*], [x], [+], [4], [/], [3], [-], [1],
            and turn it into an ordered vector like:
            [3], [*], [x], [=], [T1], [4], [/], [3], [=], [T2], [T1], [+], [T2], [=], [T3], [T3], [-], [1], [=], [T4]
            now I need to go from left to right saving the value of 3*x as t1, 4/3 as t2, then t1+t2 as t3, then t3- 1 as t4, and return t4
            */
        }
    }

    return orderedVector; 
}


//Begining of the parser and AST builder
std::vector < std::string > parserast(std::vector < std::string > inputVector) {

    std::vector < std::string > outputVector;

    if (config::debugMode) { //prints the inputVector for the cleaner to the console if debug is true
        std::cout << "Parser Started Successfully" <<std::endl;
    }

    std::vector < std::string > calcVector; //creates the vector where the correctly ordered functions and their calculations will be kept.
    
    /* for every position check for the operators from lowest to highest precedence. when one is found put the operator in Branch(1) then the fuctions to the left and right in leaf(1), 
    then find the next operator and put it in branch(2) and the values to the left and right in leaf(2), then keep going.
    */

    int operatorCount = operatorCounter(inputVector); //this calls the operator count function from the utils
    int opCounter = 1; //this string is to keep track of replacements as we go through the multipass token reducer
    calcVector = pemdas(inputVector, operatorCount, opCounter);

    if (config::debugMode) { //prints the inputFunction for the cleaner to the console if debug is true
        std::cout << "Parser Finished Successfully" <<std::endl;
    }
    
    return calcVector;
}