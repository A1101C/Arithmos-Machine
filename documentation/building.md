# Building
## Building the binaries manually:
**Generally**

You can bypass the need to use cmake or use a CMakeLists.txt file by talking directly to the compiler with a command like:
```BASH
g++ <source directory> -I<include directory> -o BINARY_NAME
```
This calls the g++ compiler and tells it to grab the contents of the source directory, to Include the include directory and output the compiled binary as BINARY_NAME.

**Specifically For This program**

An example of of a build command run from the project root directory for this program could be:
```BASH
g++ src/*.cpp -Iinclude -o ./output/engine
```
It is important to note that the `./output` folder must exist before you run this command. You can also pass different compile flag options to this. They include options like:

### g++ compiler flags
|  Use |  Flag  |  Description  |
|--------------------|--------------------|--------------------|
|  Debugging  |  -g  |  Emites DWARF debug info the binary for use with gdb  |
|  Optimization Levels  |  -O0 -O1 -O2 -O3  |  -O0 is the lowest level of optimization and -O3 is the highest, this can reorder and delete or duplicate sections of the compiled assembly language to increase speed at the cost of increased binary sizes.  |
|  Smaller Binary Sizes  |  -Os -Oz  |  Optimizes for smaller binaries at the cost of slower execution speed. -Oz optimizes very aggressively for the absolute smallest size.  |
|  Speed  |  -Ofast  |  Allows the compiler to ignore strict rules for floating point arithmetic to speed up execution speed at the cost of reduced accuracy with floating point arithmetic. Not recommended for a calculator.  |
|  Inserting or #define  |  -D  |  you can pass other objects to the compiler by using -D<object>.  |
|  DNEBUG  |  -DNEBUG  |  SEE ABOVE for -D. -DNEBUG passes `#define NDEBUG` to the top of all the .cpp files to exclude any assert() code from compilation. assert is used to check code while developing and having an assert statement in a live environment can cause the program to die if the assert statement fails its check.   |
|  Warnings  |  -Wall -Wextra -Wpedantic -Werror -Wconversion  |  -Wall enables all the most common warnings. -Wextra enables extra warnings. -Wpedantic enforces strict ISO C++ compliance and warns about any non standard extensions. -Werror treats all warnings as errors which will cause compilation to fail. -Wconversion warns if we might have a type conversion for example turning a double into an int.  |
|  Memory and Bug Cleaners  |  -fsanitize=address -fsanitize=undefined  |  Meant to be used with -g and they do slow down execution speed by quite a bit. Address catches memory leaks or out of bounds access and dangling pointers. Undefined catch undefined behavior like dividing by zero.  |
|  Linking  |  -l<library> -L<dir>  |  -l lets us link a specific system library. -L points at where the linked libraries are.  |
|  Native Compilation  |  -march=native  |  Compiles the machine code with specific optimizations for the exact CPU doing the compilation, it is not reccomended to distribute a binary compiled in this way.  |

#### Using the flags
```BASH
g++ <FLAGS HERE> -Iinclude src/*.cpp -o ./output/projectBinary
```
Example:
```BASH
g++ -Wall -Wextra -O3 -Iinclude src/*.cpp -o ./output/projectBinary
```

## Using CMake
Before you can use CMake you must make a CMakeLists.txt file, generally at the root of your project directory. A basic CMakeLists.txt file might look like:
```ini
#A minimum version is required at the top
cmake_minimum_required(VERSION 3.15)

#Then you have to give the project a name
project(project CXX)

#Set the C++ standard to use
set(CMAKE_CXX_STANDARD 17)

#force it to use that standard and prevent it from potentially falling back to an older version
set(CMAKE_CXX_STANDARD_REQUIRED True)

#grab all the src files and refer to them as BUILD_SOURCES
file(GLOB BUILD_SOURCES "./src/*.cpp")

#define the executable and the BUILD_SOURCES
add_executable(projectBinary ${BUILD_SOURCES})

#point to headers
target_include_directories(projectBinary PRIVATE include/)

#add compiler flags
target_compile_options(engine PRIVATE -Wall -Wextra -O3)
```

Next you have to configure your CMake build cache with:
```bash
cmake -s . -B build
```
This is ran from your project root directory where CMakeLists.txt is sitting. It reads the CMakeLists.txt file and generates its Makefiles automatically in the build folder. The Makefiles are like instructions on how to compile the binary.

Finally you can build your binary:
```bash
cmake --build build
```
This reads the build instructions generated during the cmake configuration step. Now any time you edit a file in your src folder you just have to run this last step again and it will recompile only the files that changed.

## Using launch configurations and tasks in vscode.

**Generally**

VS Code uses two JSON files located in a `.vscode` folder in your project root to automate compiling and debugging. `tasks.json` automates your terminal build commands, and `launch.json` tells the debugger how to run the compiled binary. By linking them together, you can compile and run your code with a single keystroke.

### Automating the build with tasks.json

Instead of manually typing `cmake --build build` every time you make a change, you can define it as a reusable task. A basic `tasks.json` for our CMake build setup might look like this:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "CMake Build",
            "type": "shell",
            "command": "cmake --build build",
            "args": [],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        }
    ]
}
```

This tells VS Code to open a shell terminal and run the `cmake --build build` command. You could also split it up and in the `"command?` line have only `cmake` and then put `"--build",` and `"build"` arguments like:
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "CMake Build",
            "type": "shell",
            "command": "cmake",
            "args": [
                "--build",
                "build"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        }
    ]
}
```
 Because `"isDefault": true` is set under the build group, you can trigger this automatically at any time by pressing `Ctrl` + `Shift` + `B`.

### Hooking up the debugger with launch.json

Once the binary is built, you need to tell VS Code where the executable is located and how to attach a debugger to it. A basic `launch.json` to launch `gdb` looks like this:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Project Binary",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/projectBinary",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                }
            ],
            "preLaunchTask": "CMake Build"
        }
    ]
}

```

**Important Notes**

There are a few critical settings in the `launch.json` file that make the automation work:

* `"program"`: This points exactly to where your final binary is output. If your `CMakeLists.txt` is generating `projectBinary` inside the `build` folder, this path must perfectly match that location.
* `"MIMode"`: This tells VS Code which backend debugger to use. For C++ development on Linux, this is generally set to `gdb`.
* `"setupCommands"`: This is where you can pass specific configurations to the debugger on startup.
* `"preLaunchTask"`: This is the most important part of the launch configuration. It links directly to the `"label"` from your `tasks.json`. Whenever you press `F5` to start debugging, VS Code will see this line and automatically run your `CMake Build` task first, ensuring your binary is always freshly compiled with your latest code changes before it runs.

# Using my Automation.

## The Build System
Using a combination of CMake and Python to manage our compilation. The Python script handles executing terminal commands, while CMake serves as the blueprint for the compiler.

Our `CMakeLists.txt` is structured with an if statement to determine if it will compile either the wasm or the regular binary. 

```ini
if(EMSCRIPTEN)
    message(STATUS "WebAssembly Build via Emscripten...")
```

This works because the Emcripten wrapper injects the `EMCRIPTEN` variable into CMake. If `EMSCRIPTEN` is detected, it makes the `evaluate.js` glue and compiles the `evaluate.wasm` binary. Otherwise, it builds the desktop binary and the gTest build.

To automate all of this `build_and_test.py` is used to build both the desktop binary with gTest to run tests and the wasm binary: .

* If the program returns any code other than 0 it failed or ran into an error, and the script uses `sys.exit(1)` so we dont run any more broken code.

## Using tasks.json

Our `tasks.json` defines several commands that execute in the shell for different levels of testing or debugging during development.

### Configured Tasks

|  Label  |  Description  |
|  ---  |  ---  |
|  Fast Compile Engine Only  |  Use this command to build the cmake configuration for the native binary.  |
|  Full Build with gTest |  Executes `python3 ${workspaceFolder}/tests/build_and_test.py` in the shell.  |
|  Fresh Native Cache Configure  |  This is a sub task that runs before another task to force a fresh rebuild of the build cache for the native binary.  |
|  Fresh Full Cache Configure  |  Forces a fresh rebuild of the build cache for both the native build and the wasm build.  |
|  Fresh Compile Engine Only  |  Use this command to build the cmake configuration for the native binary with a fresh cache.  |
|  Full Fresh Build with gTest  |  Executes `python3 ${workspaceFolder}/tests/build_and_test.py` in the shell after building a fresh cache for both builds.  |

For all of these tasks, we use `"problemMatcher": "$gcc"`. This tells vscode to put erros and warnings in the problems tab. Tasks like `Fresh Compile Engine Only` use `"dependsOn"`, which ensures the fresh cache configuration sub task runs before the build task.

---

## Using launch.json

The launch configurations connect the VS Code debugger to the binaries compiled by our tasks, allowing us to step through the code, inspect memory, and read output.

Our `launch.json` is set up with multiple similar profiles using our different tasks above to make the binaries to debug. They all use much of the same code:

```json
	{
		"name": "C/C++: g++ <name matching its task here>,
		"type": "cppdbg",
		"request": "launch",
		"program": "${workspaceFolder}/output/engine", //this is what we are pointing the debugger at
		"args": ["graph(3x+1)", "-10", "10", "20"], //we want to feed it this while debugging
		"stopAtEntry": false, //we can have it stop at entry if we want to click through and watch it work
		"cwd": "${workspaceFolder}", //just specifies our current working directory as the projects root directory for relative pathing
		"externalConsole": false, //true will run the program in a new terminal window, false runs it in the terminal at the bottom of code
		"MIMode": "gdb", //tells the cppdbg what debugger we are using
		"miDebuggerPath": "/usr/bin/gdb", //explicitly points MIEngine at gdb
		"setupCommands": [ //injects commands into gbd when called by MIEngine
			{
			"description": "Enable pretty-printing for gdb",
			"text": "-enable-pretty-printing",
			"ignoreFailures": true //this allows the debugger to give us more easily readable output
			},
			{
			"description": "Set Disassembly Flavor to Intel",
			"text": "-gdb-set disassembly-flavor intel",
			"ignoreFailures": true //makes sure we see the assembly code in Intel Syntax
			}
		],
			"preLaunchTask": "<task matching its name here>" //call this task in tasks.json and runs it before debugging
	},
```
**Note:** Vscode works pretty well when parsing json text with comments but its not guarenteed to work if its called by some other task.