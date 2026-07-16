#include <iostream> //required for any basic input/output operations
#include <cctype> //needed for isalpha
#include <string> //imports the string library
#include <vector> //imports the vector
#include <map> //includes the map
#include "config.h" //includes the config file containing debug variables
#include "utils.h" //imports my utilities
#include <cmath> //I am using this for exponents of weird numbers for now


double evaluator(std::vector < std::string> orderedVector) { //define the evaluator

    std::map<std::string, double> variableStore; //this map stores a list of strings and what double they are equal to
    double solution = 0.0; //initialize a double to hold the solution

    for ( int n = 0; n < orderedVector.size(); n += 5) { //grabs n for every 5 grouping set of strings in the orderedVector

        //sets all 5 parts of a section of an evaluator equal to some temp values
        std::string leftString = orderedVector[n]; //the first time left and right strings will be numbers but after that they can be T1, T2, ect ect
        std::string opSymbol = orderedVector[n + 1];
        std::string rightString = orderedVector[n + 2];
        std::string equalSign = orderedVector[n + 3];
        std::string targetValue = orderedVector[n + 4]; //this will always be some value T[n]

        double leftNum = 0.0; //initializes and clears the left number
        if (variableStore.count(leftString)) { //if left string is some number T[n] saved in the variableStore
            leftNum = variableStore[leftString]; //set left number equal to the stored double in variableStore
        }
        else { //if some number t[n] does not exist in variable store then leftString must be some actual number like 4 or 762
            leftNum = std::stod(leftString); //stod converts the text value inside the string into a double and saves it to leftNum
        }

        double rightNum = 0.0; //initializes and clears the right number
        if (variableStore.count(rightString)) { //if right string is some number T[n] saved in the variableStore
            rightNum = variableStore[rightString]; //set right number equal to the stored double in variableStore
        }
        else { //if some number t[n] does not exist in variable store then rightString must be some actual number like 4 or 762
            rightNum = std::stod(rightString); //stod converts the text value inside the string into a double and saves it to rightNum
        }

        double tempResult = 0.0; //initializes a temporary double to store the value of our calculation
        
        /*if (opSymbol == "^" && 1 < static_cast<int>(std::floor(rightNum))) { // if its exponential and to a higher power than 1
            double tempResult = 1.0; //set tempResult to 1 so we can use it in multiplication
            int power = static_cast<int>(std::floor(rightNum)); //make the power a whole integer rounded down
            for ( int n = 0; n < power; n++) { //for as many times as the exponent power
                tempResult *= leftNum; //save it as the temp result and multiply it by leftNum in the for loop
            }
        }
        else */

        if (opSymbol == "^") { //if its not a whole integer for the power I will figure that out later
            tempResult = std::pow(leftNum, rightNum);
        }

        if (opSymbol == "sin") { //if its a trig function then we should have trigfunctions 7 or something 
            tempResult = std::sin(rightNum); //this passes the trigfunctions and number to the hardware level trig calculators
        }
        if (opSymbol == "cos"){
             tempResult = std::cos(rightNum);
        }
        if (opSymbol == "tan"){
             tempResult = std::tan(rightNum);
        }
        
        //if the opSymbol is an inverse trig function that is just 1.0/standard trig function of right number
        if (opSymbol == "sec") {
            tempResult = 1.0 / std::cos(rightNum); 
        }
        if (opSymbol == "csc") {
            tempResult = 1.0 / std::sin(rightNum); 
        }
        if (opSymbol == "cot") {
            tempResult = 1.0 / std::tan(rightNum); 
        }

        if (opSymbol == "log") { //if it is a log function we pass it through as base 10 to the hardware level log calculator
            tempResult = std::log10(rightNum);
        }
        
        if (opSymbol == "ln") {
            tempResult = std::log(rightNum); //this passes log base e or ln functions to the hardware calculator
        }

        //these all evaluate operators as normal by using machine language commands as this is as far down as I can go in C++
        if (opSymbol == "*") tempResult = leftNum * rightNum;
        if (opSymbol == "/") tempResult = leftNum / rightNum;
        if (opSymbol == "+") tempResult = leftNum + rightNum;
        if (opSymbol == "-") tempResult = leftNum - rightNum;

        variableStore[targetValue] = tempResult; //store our temp result in the variableStore targetValue which will be some number T[n] so we can use it for the next step of the evaluation

        solution = tempResult; //saves the temp result every time as solution until the loop finishes on our final T[n]

    }

    return solution;
}


double interpreter(std::vector < std::string > orderedVector) { //define the interpreter
    double solution = 0.0; //initialize a double to hold the solution

    if (config::debugMode) { //prints the following if debug is true
        std::cout << "interpreter Started Successfully" <<std::endl;
    }

    solution = evaluator(orderedVector);

    if (config::debugMode) { //if debugmode
        std::cout << "Interpreter finished with: " << solution << " \n";
    }

    return solution;
}