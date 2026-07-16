# How The Compiled Engine Works.

#### Quick notes:
When talking about tokens I type them out contained in brackets and separated by commas like: [12], [+], [12], but in the actual program they are stored without them, I just print them in the debug info like that.

## Passing arguments into the binary.
The entry point for the function is main() in main.cpp. It accepts arguments in a raw array containing c-strings. The first part of the array is expected to be the type of calculation and the expression being evaluated wrapped in parenthesis, such as:
```
sci(12+56)
```
or:
```
graph(1/2x+56)
```
or you can defined the bounds of your graph, with its minimum x value, its maximum x value and the number of x values to calculate with:
```
graph(1/2x+56) -10 10 20
```
The next parts of the raw array are launch flags, currently -d or -s.
```
sci(12+56) -d -s
```
 -d is used to enable debugMode and print output that is useful for debugging the program. -s is used to enable showSteps which will be used to give a pedagogical print out of how the calculation is preformed.

### What the raw array looks like.
The main function is defined as:

```
int main(int argCount, char*argVector[]) {
```

This means it returns an integer upon completion and accepts any string of characters as input with positions in the array separated by spaces. for example if you gave it sci(12+56) -d -s it would make save sci(12+56) in argVector[1] and then -d in argVector[2] ect ect...

In other words if you called the binary with:
```
./engine graph(1/2x+56) -d -s
```
Then printed each part of the array argVector[n] for n starting at 0 and increasing as long as it is less than argCount then it would return:
```
argVector[0] as storing ./engine

argVector[1] as storing graph(1/2x+56)

argVector[2] as storing -d 

argVector[3] as storing -s
```

## Parsing the expression.
The first thing the main function does is check if argCount is greater than two and if it is then it checks the last two c-strings at the end of the raw array. If either of them are -d or -s it sets the appropriate boolean as true. This is important so that if you are debugging the first thing the function does is sets debugMode as true so you can see exactly where the program crashed.

It then looks at the argVector[1] and parses it into a "messyFunction" which I define as whatever function the user inputs and into a "calcType" which is the type of calculation being performed. Right now it supports scientific calculations sci(expr) like you would do on a scientific calculator with trigonometric functions and logarithmic functions and graphing with graph(expr) with the same support as scientific calculations.

### Turning the input into something more useful.
The way the c-string sci(12+56) is parsed into its constituent parts is by converting the c-string, argVector[1], into a cpp standard library string class object, the variable named "rawString", from here on simply called a string. This allows us to ask questions about it like its length. 

We then start at the left most position of the rawString and appends every character in a string variable called tempString as long as the current character is not "(" because contained in the () should be our expression. When it hits "(" it then puts the values stored in tempString into the calcType string and clears tempString. It then moves passed the open parenthesis, and begins to append the characters into tempString again until we hit a ")" marking the end of the expression. Then the tempString is stored in our messyFunction and tempString is cleared again.

Now we have our calcType which is either graph or sci, and our messyFunction which is whatever expression the user wanted evaluated.

### Passing the expression to the rest of the calculator.
Once the messyFunction has been pulled out of our raw input we can pass that directly into the cleaner to get our cleanFunction, which we pass into the lexer to get a vector of strings called the tokenizedFunction which we pass into the parser to get our parsedFunction.

Below is a brief Explanation of how the different parts of the calculator work and below them I will go into greater detail.

### The Cleaner

The cleaners jobs is to make sure the function is cleaned of errors or values the calculator cant use. It makes sure there are no spaces " " and that if the function is just x it is replaced with 1x. The cleaner will be used in the future to expand the error handling ability of the calculator by ensuring that edge cases are cleaned into something the rest of the program can recognize.

### The lexer

The lexers job is to turn everything into a "token" because a computer has no idea what 12+56 is. So the lexer breaks this into its constituent parts so we can use it later. It turns one string into a vector of strings like [12], [+], [15] which we can then use to pass to the computers processor later to calculate.

### The Parser
The job of the parser is to put everything in order so that when we actually calculate our expression it follows the order of operations. There are quite a few ways to create a parser the way I stumbled upon while trying to make my own is what is called a multi pass reduction parser. 

## Tokenizing and Parsing the expression.
### Tokenizing

The lexer works from left to right, tokenizing by simply checking what kind of data we are currently looking at and what kind of data is next. It does this by looping through the entire input function string.

To pull out numbers that exceed one index position like 12.5 we simply ask:

Is the current character, currentChar, at position inputFunction[n] is a digit? if it is we add it to a temporary token called currentToken. Then since we know we started looking at a number we can look at the next position of n, and if its a number or a decimal place then we add it to the currentToken. Only once we are no longer looking at a number or a decimal place we can push the currentToken into the lexedTokens vector, clear the currentToken and then ask, What is the next character?

Is currentChar alphabetical? If so then we could be looking at either an 'x' or maybe a trig function like 'sin' so to know we need to evaluate what the next three currentChar's are toghether as nextThree. We then check if they are contained in our list of trig functions or logarithmic functions and if they are we add the function to the lexedTokens and clear the currentToken.

but if currentChar is an x or X we normalize X to x and then add it to the lexed tokens and check if the lastCharacter was a digit, and if it was we insert a [*], token between them so that coefficients are multiplied correctly.

Is the currentChar contained in our list of Operators? If so simply add it to the lexedTokens.

### Parsing
To demonstrate how it works imagine the lexer returned and expressions vector of tokens such as:
```
[(], [12], [+], [12], [)], [*], [2],
```
When this is past into the parser it looks first for any parenthesis, by starting from the right and moving to the left looking for and open parenthesis "(", this method allows it to find the inner most open parenthesis, it then moves from that position to the right looking for a closed parenthesis ")". Once it knows the position of our inner most and outer most parenthesis it is able to extract everything within them and assign them to a temporary token. The tokens follow the pattern T[n] where n is the token number we are on starting at one and counting up. 

so we would still have our inputVector with:
```
[T1], [*], [2],
```
and our orderedVector would have the contents of T1 as:
```
[12], [+], [12], [=], [T1]
```

It then passes the values it declared as T1 to the parser where they are organized based on the order of operations using a vector of vectors containing the different operators:
```
    std::vector < std::vector < std::string > > operatorPriority = { //this is a vector of vectors, the inner vectors contain equal priority operators
        {"sin", "cos", "tan", "sec", "csc", "cot", "log", "ln"}, //Tier 0
        {"^"},          //Tier 1 
        {"*", "/"},     //Tier 2
        {"+", "-"},     //Tier 3
        {"="},          //Tier 4
    };
```
The program then looks through the inputVector and looks for operators in multiple passes, first looking for operators in tier 0, then 1, and so on and so on. This ensures that it grabs the tokens in the correct order. When it finds an operator it grabs the operator token and the tokens to the left and right of it replacing all three with a new placeholder token Tn. This slowly reduces the size of the input vector by adding the parts we are pulling out to an ordered vector we can evaluate from left to right.

This means our example input of:
```
[(], [12], [+], [12], [)], [*], [2],
```
Will be parsed to:
```
[12], [+], [12], [=], [T1], [T1], [*], [2], [=], [T2],
```
This ensures the items inside the parenthesis are evaluated first and then multiplied by 2.

## Evaluation
The evaluator is really quite simple, since the parser breaks any function into multiple sections of five strings we can evaluate them as such. If given:
```
[12], [+], [12], [=], [T1], [T1], [*], [2], [=], [T2],
```
It grabs the first 5 strings of the vector and checks if index 0 and 2 are variables Tn, if so they keep Tn in the equation, if not it converts the number held as a string to a double. It then looks at the index in the 1 position and makes that our opSymbol or operator. It then uses if statements to find the correct operation to preform and then calculates 12+12 saving it as the variable in the index 4 position so that we can use that variable for the next part where it does the same checks but passes T1, holding the result of 12+12, *2 to the CPU to be calculated. Once we have gone through every 5 string chunk in the inputVector we can return the last Tn value as our solution.