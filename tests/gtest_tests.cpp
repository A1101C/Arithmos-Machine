//Write tests in here for each part of the calculator

//gtest import
#include <gtest/gtest.h>
//library imports
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstddef>
//local function imports
#include "config.h"
#include "cleaner.h"
#include "lexer.h"
#include "parserast.h"
#include "interpreter.h"
#include "graphpoints.h"


namespace basicTestAdd {

    std::string inputBasicAdd = " 1 + 1 ";
    std::string cleanBasicAdd = "1+1";
    std::vector < std::string > tokenBasicAdd = {"1", "+", "1"};
    std::vector < std::string > parsedBasicAdd = {"1", "+", "1", "=", "T1"};
    double solutionBasicAdd = 2.0;

}

namespace basicTestGraph {
 
std::string inputBasicGraph = " 1 / 4 x + 1 ";
std::string cleanBasicGraph = "1/4x+1";
std::vector < std::string > tokenBasicGraph = {"1", "/", "4", "*", "x", "+", "1"};
std::vector < std::string > parsedBasicGraph = {"4", "*", "x", "=", "T1", "1", "/", "T1", "=", "T2", "T2", "+", "1", "=", "T3"};
double xMin = -10;
double xMax = 10;
double xCount = 20;
std::vector < std::pair < double, double >> outputGraphExpected = {
    {-10, -1.5},
    {-9,  -1.25},
    {-8,  -1},
    {-7,  -0.75},
    {-6,  -0.5},
    {-5,  -0.25},
    {-4,  0},
    {-3,  0.25},
    {-2,  0.5},
    {-1,  0.75},
    {0,   1},
    {1,   1.25},
    {2,   1.5},
    {3,   1.75},
    {4,   2},
    {5,   2.25},
    {6,   2.5},
    {7,   2.75},
    {8,   3},
    {9,   3.25},
    {10,  3.5}
};
}

//sanity check to make sure the most basic of math works in the evaluator
TEST(basicTest, basicInterpreter) {
    std::vector < std::string > input = basicTestAdd::parsedBasicAdd;
    double output = interpreter(input);
    double outputExpected = basicTestAdd::solutionBasicAdd;
    EXPECT_DOUBLE_EQ(output, outputExpected);
}
 
TEST(componentTestManual, basicCleaning) { //Tests the cleaner in isolation
    auto input = basicTestAdd::inputBasicAdd;
    auto output = cleaner(input);
    auto outputExpected = basicTestAdd::cleanBasicAdd;
    EXPECT_EQ(output, outputExpected);
}
 
TEST(componentTestManual, basicTokenization) { //tests the lexer in isolation
    auto input = basicTestAdd::cleanBasicAdd;
    auto output = lexer(input);
    auto outputExpected = basicTestAdd::tokenBasicAdd;
    for( int n = 0; n < output.size(); n++){
        EXPECT_EQ(output[n], outputExpected[n]);
    }
}
 
TEST(componentTestManual, basicParsing) { //tests the parser in isolation
    auto input = basicTestAdd::tokenBasicAdd;
    auto output = parserast(input);
    auto outputExpected = basicTestAdd::parsedBasicAdd;
    for( int n = 0; n < output.size(); n++){
        EXPECT_EQ(output[n], outputExpected[n]);
    }
}
 
TEST(fullRunManual, basicSci) { //tests  every part of the pipeline to ensure we can solve simple scientific calculations
    auto input = basicTestAdd::inputBasicAdd;
    auto cleanOutput = cleaner(input);
    auto tokenOutput = lexer(cleanOutput);
    auto parsedOutput = parserast(tokenOutput);
    auto output = interpreter(parsedOutput);
    auto outputExpected = basicTestAdd::solutionBasicAdd;
    EXPECT_DOUBLE_EQ(output, outputExpected);
}

TEST(fullRunManual, basicGraph) { //tests every part of the pipeline to ensure we can graph a simple function
    auto input = basicTestGraph::inputBasicGraph;
    auto cleanOutput = cleaner(input);
    auto tokenOutput = lexer(cleanOutput);
    auto parsedOutput = parserast(tokenOutput);
    std::vector < std::pair < double, double >> output = graphpoints(parsedOutput, basicTestGraph::xMin, basicTestGraph::xMax, basicTestGraph::xCount);
    std::vector < std::pair < double, double >> outputExpected = basicTestGraph::outputGraphExpected;
    for( int n = 0; n < output.size(); n++){
        EXPECT_EQ(output[n], outputExpected[n]);
    }
}
