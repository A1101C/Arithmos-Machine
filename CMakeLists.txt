
cmake_minimum_required(VERSION 3.14) #minimum required version for for gTest features
project(Explicanda_NATIVE_AND_WASM CXX) #defines the project name as ExplicandaWASM and states it is written in cpp

set(CMAKE_CXX_STANDARD 17) #tells the compiler to use the c++ 17 standard from 2017
set(CMAKE_CXX_STANDARD_REQUIRED ON) #forces the compiler to use c++ 17 so it cant fall back to an older version

if(NOT CMAKE_BUILD_TYPE) #this tells cmake to set the build type to debug unless otherwise specified
    set(CMAKE_BUILD_TYPE Debug)
endif()

#set up build flags for the native build
set(NATIVE_COMPILE_FLAGS
    "-g" #generate debug info for gdb
    "-Wall" #enables standard warnings
    "-Wextra" #enables extra warnings
    "-Wpedantic" #forces absolute C++17 compliance and issues a warning if non standard libraries are used so I know what wasm is guaranteed to have access to
)

#set up the link flags for wasm
set(WASM_LINK_FLAGS
    "-O3" #maximize optimization
    "-sALLOW_MEMORY_GROWTH=1" #allow the program to go beyond emscripten's default memory size limit
    "-sASSERTIONS" #enables emscripten's own checks, disable for final deployment
    "-sEXIT_RUNTIME=0" #prevents the wasm module from being completely unloaded when it finishes a command
    "-sMODULARIZE=1" #wraps the glue code inside its own function to keep variables separate from global variables
    "-sEXPORT_ES6=1" #makes the wrapped module standard es6 importable so we don't have to use a script to import it
    "-sINVOKE_RUN=0" #prevents the binary from being ran on import with no data passed to it
    "-sEXPORTED_FUNCTIONS=['_main','_fflush']" #these let us flush the wasm stdout buffer between executions
    "-sEXPORTED_RUNTIME_METHODS=['callMain','ccall','cwrap']" #other methods to call main, currently only callMain is used to pass the arguments into the wasm
)

#define directories relative to the CMake file
set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}") #saves the project root dir relative to this file

#sources and include paths
set(INCLUDE_DIR "${PROJECT_DIR}/include")
set(SRC_DIR "${PROJECT_DIR}/src")
set(TESTS_DIR "${PROJECT_DIR}/tests")

#output paths
set(NATIVE_DIR "${PROJECT_DIR}/output") #put the regular desktop binary here
set(WASM_DIR "${PROJECT_DIR}/portal/src/wasm") #save the evaluate.js file here
set(WASM_PUBLIC_DIR "${PROJECT_DIR}/portal/public/wasm") #put the compiled evaluate.wasm here

#collect the source files from the path
file(GLOB ENGINE_SOURCES "${SRC_DIR}/*.cpp") 
list(REMOVE_ITEM ENGINE_SOURCES "${SRC_DIR}/main.cpp") #removes the main.cpp file from the gtest compilation

#this is the fork in the road where it will compile either the wasm or the regular binary
if(EMSCRIPTEN) #when running emccmake cmake it will inject a variable telling cmake to use the emcc and evaluate this statement to true
    message(STATUS "WebAssembly Build via Emscripten...")

    add_executable(evaluate ${ENGINE_SOURCES} "${SRC_DIR}/main.cpp") #this makes the evaluate.js glue and compiles the evaluate.wasm binary from the engine sources and pulls the main back in for this compilation
    target_include_directories(evaluate PRIVATE ${INCLUDE_DIR}) #this tells the compiler to include these directories

    #include the link flags defined at the top of this file
    target_link_options(evaluate PRIVATE ${WASM_LINK_FLAGS})

    #put the output files in the wasm_dir
    set_target_properties(evaluate PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${WASM_DIR}")

#building the desktop binary
else()
    message(STATUS "Desktop Binary & GTest Build...")

    add_executable(engine ${ENGINE_SOURCES} "${SRC_DIR}/main.cpp") #grabs the main to pull it back into the compilation
    target_include_directories(engine PRIVATE ${INCLUDE_DIR}) #includes the include directory

    #set the compilation flags for the native desktop build
    target_compile_options(engine PRIVATE ${NATIVE_COMPILE_FLAGS})

    #puts the regular binary in the native_dir folder
    set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${NATIVE_DIR}")
    
    #this option gets passed in by the python scripts -DENABLE_TESTS flag, -D passes things into the CMakeBuild Config
    option(ENABLE_TESTS "Enable building gTest" OFF)

    if(ENABLE_TESTS)

        # this grabs the google gTest content so we can integrate that set of tests into our program
        include(FetchContent)
        FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        )
        FetchContent_MakeAvailable(googletest)

        # Builds the gTest Executable
        enable_testing()
        
        add_executable(gtest_tests "${TESTS_DIR}/gtest_tests.cpp" ${ENGINE_SOURCES}) #says to make the gtest_tests executable and use gtests_test.cpp and the engine sources
        target_include_directories(gtest_tests PRIVATE ${INCLUDE_DIR}) #includes the include folder in the build
        target_link_libraries(gtest_tests PRIVATE gtest_main) #uses the gTest library to put a gtest_main() function in the binary

        #Put the test Binary in the native_dir
        set_target_properties(gtest_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${NATIVE_DIR}")
        
        include(GoogleTest) #this tells CMake to look inside the compiled unit_tests binary and register each TEST()
        gtest_discover_tests(gtest_tests)

    endif()
endif()