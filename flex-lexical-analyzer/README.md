# Flex Guide
A complete guide for using Flex to generate a lexical analyzer for the given regular expressions

## Table of Contents
- [Installation](#installation)
- [Writing a Flex Program](#writing-a-flex-program)
- [Running the Program](#running-the-program)


## Installation
1. Install [Flex](https://gnuwin32.sourceforge.net/packages/flex.htm).
2. Install compiler for C- [MinGW](https://sourceforge.net/projects/mingw/files/latest/download?source=files).

## Writing a Flex Program
1. Create file `lexical_analyzer.l`.
2. The program is divided into 3 sections: **definitions**, **rules**, and **functions** where we will add the `main` procedure:
    ````
    %{
    
        /* definitions */
    
    %}
    
    
    %%
    
        /* rules */
    
    %%
    
        /* functions */
    ````
3. Type rules for regular expressions inside the `rules` section in the form of `pattern action` where the priority is defined by order:
    ````
    %%
   
         /* rules */

    "boolean"               { printf("boolean"); }
    "int"                   { printf("int"); }
    "float"                 { printf("float"); }
    "if"                    { printf("if"); }
    "else"                  { printf("else"); }
    "while"                 { printf("while"); }
    [a-zA-Z][a-zA-Z0-9]*    { printf("id"); }
    [0-9]+|[0-9]+\.[0-9]+   { printf("num"); }
    \=\=|!\=|>|>\=|<|<\=    { printf("relop"); }
    =                       { printf("assign"); }
    \+|-                    { printf("addop"); }
    "\*"|"/"                { printf("mulop"); }
    [;|,|\(|\)|{|}]         { printf("%s", yytext); }
    " "                     { printf("%s", yytext); }
    \n                      { printf("%s", yytext); }
    .                       { /* Unmatched pattern */ }
   
    %%
    ````
4. Define the `main` function which reads input file, matches patterns, then writes the matched tokens to the output file:
    ````c++
    int main(int argc, char **argv) {
      if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
      }
    
      // Open the input file
      FILE *inputFile = fopen(argv[1], "r");
      if (!inputFile) {
          perror("Error opening input file");
          return 1;
      }
    
      // Open the output file and redirect stdout to it
      FILE *outputFile = freopen(argv[2], "w", stdout);
      if (!outputFile) {
          perror("Error opening output file");
          fclose(inputFile);
          return 1;
      }
    
      // Set the input file for lexing
      yyin = inputFile;
    
      // Run the lexer
      yylex();
    
      // Close the files
      fclose(inputFile);
      fclose(outputFile);
    
      return 0;
    }
    ````
5. Create file `in.txt` which contains the program input to the lexical analyzer:
    ````text
   int sum , count , pass , mnt; while (pass !=10)
    {
    pass = pass + 1 ;
    }
    ````
## Running the Program
1. In the terminal, type the following:
   ````commandline
    flex lexical_analyzer.l
    gcc lex.yy.c -o output
    .\output in.txt out.txt
   ````
2. Opening `out.txt`:
    ````text
    int id , id , id , id; while (id relopnum)
    {
    id assign id addop num ;
    }
    ````
