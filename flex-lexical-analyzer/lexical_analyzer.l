%{

    /* definitions */

%}


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

int yywrap() {}

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