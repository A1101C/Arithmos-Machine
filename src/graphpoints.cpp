#include <iostream> //required for any basic input/output operations
#include <cctype> //needed for isalpha
#include <string> //imports the string library
#include <vector> //imports the vector
#include <cmath> //used for the round function
#include <sstream> //required for stringstream
#include "config.h" //includes the config file containing debug variables
#include "utils.h" //this has the containsString function
#include "interpreter.h" // includes my interpreter

//this tool allows me to round a double value based on my epsilon precision, it is called a floating point snapping utility

double roundToEpsilon(double value, double epsilon) {
        return round(value / epsilon) * epsilon;
}

std::string doubletoString(double d) { //convert a double to a string
    std::ostringstream oss; //this creates a buffer called ostringstream
    oss = std::ostringstream(); //this will guarantee that it is empty when I try to use it again by assigning an empty object to it
    oss << d; //throws the double into the buffer
    return oss.str(); //then retrieves the data as a string
}

std::vector < std::pair < double, double >> graphpoints(const std::vector < std::string> parsedFunction, 
                                                                    double xMin, 
                                                                    double xMax, 
                                                                    double xCount) {

    if (config::debugMode){
        std::cout << "GraphPoints Started Successfully" <<std::endl;
    }

    using utils::epsilon;  //pulls epsilon from the utils namespace into this file

    std::vector < std::pair < double, double >> xyPairs; //initializes the tuple to hold all the xy pairs

    double currentX; //initializes a double to hold the current x value
    double currentY; //initializes a double to hold the current y value
    double xOffset; //initializes a double to hold the current offset from the xmin

    double xRange = xMax - xMin; //defines the range of x values based on the max and min
    double xStep = xRange / xCount; //determines the step between X values based on the range and count

    std::string currentXstring; //declares an empty string to hold the current x for our function
    std::vector < std::string > xFunction; //declares an empty vector to hold the xFunction

    if (config::debugMode) { //if debug mode
        std::cout << "tokenized xFunction = ";
        for (const std::string& token : parsedFunction) {
            std::cout << token << ", ";
        }
        std::cout << std::endl;

        std::cout << "xMin = " << xMin << " \n";
        std::cout << "xMax = " << xMax << " \n";
        std::cout << "xCount = " << xCount<< " \n";
        std::cout << "xRange = " << xRange << " \n";
        std::cout << "xStep = " <<xStep << " \n";
    }

    for ( int n = 0; n <= xCount; n++ ) { //for all of the points in x count

        //this parts sets up the current value of x to feed into the interpreter
        xOffset = xStep * n; //the new x offset = the xStep times n
        currentX = xMin + xOffset; //the currentX value is is equal to the xMin + the xOffset
        currentXstring = doubletoString(currentX); //makes the currentXstring = to the double currentX converted to a string

        //this part puts the x value in the function and passes it to the replaceStrings function
        xFunction = replaceStrings(parsedFunction, "x", currentXstring); //replace the xvalue in the xFunction function with the currentXstring
        currentY = evaluator(xFunction); //sends the xFunction to the interpreter to be solved for y 

        xyPairs.push_back({currentX, currentY}); //adds our current x and y as a pair into the vector

    }

    if (config::debugMode){
        std::cout << "GraphPoints Finished Successfully" <<std::endl;
    }

    return xyPairs; //return the vector of xy pairs
}