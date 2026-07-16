# How The Compiled Engine Works.

## Passing arguments into the binary.
The entry point for the function is main() in main.cpp. It accepts arguments in a raw array containing c-strings. The first part of the array is expected to be the type of calculation and the expression being evaluated wrapped in parenthesis, such as:
```
sci(12+56)
```
or:
```
graph(1/2x+56)
```
or you can defined the bounds of your graph with:
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

It then looks at the argVector[1] and parses it into a "messyFunction" which I define as whatever function the user inputs and into a "calcType" which is the type of calculation being preformed. Right now it supports scientific calculations sci(expr) like you would do on a scientific calculator with trigonometric functions and logarithmic functions and graphing with graph(expr) with the same support as scientific calculations.

### Turning the input into something more useful.
The way the c-string sci(12+56) is parsed into its constituent parts is by converting the c-string, argVector[1], into a cpp standard library string class object, the variable named "rawString", from here on simply called a string. This allows us to ask questions about it like its length. 

We then start at the left most position of the rawString and appends every character in a string variable called tempString as long as the current character is not "(" because contained in the () should be our expression. When it hits "(" it then puts the values stored in tempString into the calcType string and clears tempString. It then moves passed the open parenthesis, and begins to append the characters into tempString again until we hit a ")" marking the end of the expression. Then the tempString is stored in our messyFunction and tempString is cleared again.

Now we have our calcType which is either graph or sci, and our messyFunction which is whatever expression the user wanted evaluated.

### Cleaning Tokenizing and organizing the expression.