#this file is used to automate building both the desktop binary with gTest to run tests and the wasm binary 

import subprocess
import os
import sys

#subprocess allows python to run external system commands, and os allows python to interact with the systems file system. Always be careful of running a script with these two imports


#Define source and output paths relative to the script 
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__)) #this is exactly the /tests directory

#project root dir and and /build dir
PROJECT_ROOT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, ".."))
CMAKE_LISTS_DIR = os.path.abspath(PROJECT_ROOT_DIR) #the CMakeLists.txt is here

#build cache folders
NATIVE_BUILD_DIR = os.path.join(PROJECT_ROOT_DIR, "build", "nativeBuild")
GTEST_BUILD_DIR = os.path.join(PROJECT_ROOT_DIR, "build", "gTestBuild")
WASM_BUILD_DIR = os.path.join(PROJECT_ROOT_DIR, "build", "wasmBuild")

#the output folders
NATIVE_OUT_DIR = os.path.join(PROJECT_ROOT_DIR, "output")
WASM_API_DIR = os.path.join(PROJECT_ROOT_DIR, "portal", "src", "wasm")
WASM_OUT_DIR = os.path.join(PROJECT_ROOT_DIR, "portal", "public", "wasm")



def run_command(COMMAND, DESCRIPTION, CWD="."): #defines the run command function so we can run commands
    print(f"\n {DESCRIPTION}...") #tells us what command it is running
    RESULT = subprocess.run(COMMAND, shell=True, cwd=CWD) #this line actually executes the command in our current working directory
    if RESULT.returncode != 0: #if the program returns any code other than 0 it failed or ran into an error
        print(f"Error: {DESCRIPTION} Failed.") #Tells us what part failed
        sys.exit(1) #exits the python script so we dont run any more broken code
    print(f"{DESCRIPTION} Succeeded!") #if the return code is 0 it will pass the previous if statement and tell us it succeeded.



def main(): #defines the actual main part of the program

    #make sure internal build cache folders exist right here in the /tests directory
    os.makedirs(NATIVE_BUILD_DIR, exist_ok=True)
    os.makedirs(WASM_BUILD_DIR, exist_ok=True)

    #make sure our output folders exist.
    os.makedirs(NATIVE_OUT_DIR, exist_ok=True)
    os.makedirs(WASM_API_DIR, exist_ok=True)
    os.makedirs(WASM_OUT_DIR, exist_ok=True)

    #configure and build the main engine in nativeBuild
    run_command(f"cmake -S {CMAKE_LISTS_DIR} -B {NATIVE_BUILD_DIR} -DENABLE_TESTS=OFF", "Configuring Native Engine")
    run_command(f"cmake --build {NATIVE_BUILD_DIR} --target engine -j", "Building Engine")

    # Configure and build the gtest tests in gtestBuild
    run_command(f"cmake -S {CMAKE_LISTS_DIR} -B {GTEST_BUILD_DIR} -DENABLE_TESTS=ON", "Configuring gTest Build")
    run_command(f"cmake --build {GTEST_BUILD_DIR} --target gtest_tests -j", "Building Tests")

    #run the gtests
    run_command(os.path.join(NATIVE_OUT_DIR, "gtest_tests"), "Running gTest on the gtest_tests binary")

    #run the wasm build command to make the evaluate.wasm and evaluate.js
    run_command(f"emcmake cmake -S {CMAKE_LISTS_DIR} -B {WASM_BUILD_DIR}", "Configuring WASM build")
    run_command(f"cmake --build {WASM_BUILD_DIR} --target evaluate -j", "Compiling WebAssembly evaluate file")

    #move the evaluate.wasm file to its public folder
    run_command(f"mv {WASM_API_DIR}/evaluate.wasm {WASM_OUT_DIR}/evaluate.wasm", f"Moving evaluate.wasm to {WASM_OUT_DIR} ")

    #Finish the script and announce where the files are located.
    print(f"\n Dual build script completed. Native output placed in {NATIVE_OUT_DIR}. The wasm files are in {WASM_API_DIR}, and {WASM_OUT_DIR}. ")

if __name__ == "__main__":
    main()

