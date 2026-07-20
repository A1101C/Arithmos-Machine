# Testing with gTest
## Integrating GoogleTest into the CMake build.
This project integrates [googleTests](https://google.github.io/googletest/), hereafter referred to as gTest, to automate the testing of the C++ calculator functions. It does this by building a separate gtest binary and replacing the main() function with a test() function and including test code that you write.

To build a gtest binary you must use at least CMake 3.14 or newer and C++17 or newer. You include it in the CMake build process with this:
```ini
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
```
This fetches the googletest code and makes it available to CMake as googletest. You can then add it to the CMake build replacing the main() function with something similar to:
```ini
add_executable(gtest_tests ./gtest_tests.cpp /src/)

target_include_directories(gtest_tests PRIVATE /include/)

target_link_libraries(gtest_tests PRIVATE gtest_main)

set_target_properties(gtest_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY -o /output)
        
include(GoogleTest)
gtest_discover_tests(gtest_tests)
```
Because gtest replaces the main() function with its test() function you must remove remove the main.cpp file from the compilation process before running the CMake build above, or not include it to begin with by leaving it out of the src folder and pointing your regular CMake build at it explicitly. You can remove it from an engine sources glob with something like:
```ini
file(GLOB ENGINE_SOURCES "${SRC_DIR}/*.cpp") 
list(REMOVE_ITEM ENGINE_SOURCES "${SRC_DIR}/main.cpp")
```

###  Building Tests
Here is a very basic example of how to test the evaluator function:

``` cpp
#include <gtest/gtest.h> //you must include the gtest.h precompiled header file

#include <iostream> //then include the same things your regular main relies on
#include <cctype>
#include <string>
#include <vector>

#include "interpreter.h" //and include the function you want to test from test()


//sanity check to make sure the most basic of math works in the evaluator
TEST(interpreterTestManual, basicAddition) {
    std::vector < std::string > tokens = {"1", "+", "1", "=", "T1"}; //the interpreter takes a vector of strings like this
    double solution = interpreter(tokens); //the interpreter returns the solution as a double
    EXPECT_DOUBLE_EQ(solution, 2.0); //this says we expect the result to be a double and that we are making sure that solution is a double equal to 2.0
}
```
You then build each test separately in their own`TEST(){}` function and when you run the compiled test binary it will run through and run every test telling you if it passed or failed.

## Using my automated workflow.
If you import my `CMakeBuild.txt` file into the project root directory, the `build_and_test.py` python script into the `tests` folder and import the contents of the example: `tasks.json` and `launch.json` files the run and debug menu in vs code will give you the options to build just the native desktop binary, or the full suite of three binaries, the native desktop binary, the gtest binary, and the wasm binary. It will also automatically execute the gtest binary so you can see right in the terminal if every test succeeded.

