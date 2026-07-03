# explicanda

Try the live tool here: https://explicanda.com/

## Project Introduction

This project is a fully custom web-based symbolic calculator. It uses Astro for the web pages and component based building and the CPP binary is compiled into WASM so that computations are run completly client side.

My goal was to improve my software development abilities and learn about computational mathematics. In order to do so I have written a fully custom C++ back end engine to clean, tokenize, parse, and finally interpret a mathematical expression, instead of relying on a premade high level math library. That is connected to a server side generated webpage made with astro that combines the front end and api routing into a single toolkit.

The project utilizes this multi language stack to optimize the computation execution speed and web accessibility. 

## Dependencies
As of right now the only dependencies are a C++ compiler, astro, and npm if you plan to build the tool yourself. Everything else has been build by me and the dockerfile actually imports g++ compiler to compile the binary for your system and builds the webpages. So the docker deployment is the easiest route. if you choose to build the binary yourself make sure your build command looks something like:

## Docker
Docker Run
```
docker run --name explicanda-container -p 4321:4321 --restart unless-stopped a1101c/explicanda:latest
```

Docker Compose
```
services:
  explicanda:
    image: a1101c/explicanda:latest
    container_name: explicanda-container
    ports:
      - "4321:4321"
    restart: unless-stopped
```

Notice this doesn't bind any volumes because we want to recompile the binary every time I update it to add a new feature or optimization. 

## Additional Notes


# Development and Goals
## History
I actually got the idea to build a symbolic math engine and graphing calculator a good little while ago, and I tried to build it in python initially but the built in eval function in python gives very little real control over how I handle an expression, it also doesn't teach me much about computational mathematics. When graphing using the built in eval function you also have to call it many times to get enough points to draw a graph, and doing so is very slow because it has to reach out to the whole imported library for every value it wants to solve. So after a while I decided to start over and build it from the ground up in C++. The journey has been very rewarding so far, I have really enjoyed learning and overcoming the challenges of designing something of this nature. It does finally support all basic arithmetic with PEMDAS, and trig functions like Sin, Cos, Tan, Sec, Csc, and Cot, as well as log and ln which is log base e.

To get to this point I had to learn how to take, clean, and validate a user input. Lex a string into tokens consisting of numbers, operators, and potentially letters. The next challenge was learning how to organize those tokens into a organized vector that positions everything according to the order of operations. I do know about shunting yard algorithm, but I wanted to try to figure out on my own how to do it and I discovered for myself what I have since learned is called a multi pass reduction parser. So rather than building an Abstract Syntax Tree (AST) where each operator creates a branch based on its order of operations compared to the adjacent ones, I create placeholder values and pull out the operator and values to either side of them organizing a vector of tokens like:
```
[3], [*], [7], [+], [4], [/], [3], [-], [1],
```
into:
```
[3], [*], [7], [->], [T1], [4], [/], [3], [->], [T2], [T1], [+], [T2], [->], [T3], [T3], [-], [1], [->], [T4]
```
This pulls out the highest priority and places it first, then the next and so on. when it gets to an operator where one of the values has already been pulled out and replaced with a placeholder it just pulls out that placeholder and uses it in its part of the final expression. The interpreter can then look at every group of 5 items in the vector, know where the operator is, know exactly what two values to preform math with, and then assign them to a variable like T1, then when T1 is later used it has a value and is calculated and treated as its number value. This seems to be very effective though I don't have a Shunting Yard algorithm to compare it against.




## Goals

This however is just the beginning, my plans are to build this out further including a graphing feature, CAS features, Calculus features for solving derivatives and Integrals, physical motion for objects with constant acceleration, and much more.

#### Checklist:
| Goals and TODO | Completed? |
| ------------- |:-------------:|
| Build basic calculator engine? | X |
| Create the basic framework for adding more pages and tools in the future | X |
| Implement feature to print the steps |  |
| Build an integrated set of tests |  |
| Implement ability to create a table of values| X |
| Implement ability to generate a graph | X |
| Implement ability to solve for a variable CAS |  |
| Implement symbolic Calculus solver for Derivatives |  |
| Implement Symbolic Calculus solver for Integrals |  |
| Implement simulation tools |  |

#### Bug Tracker:

| Known Bugs: |
| :----------: |


