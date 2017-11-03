#include <string.h>
#include <stdio.h>
#include <math.>
#include <stdlib.h>
#include <pl0data.h>

// given 
const char *lexSym[] = { "", "nulsym", "identsym", "numbersym", "plussym", "minussym",
    "multsym",  "slashsym", "oddsym", "eqsym", "neqsym", "lessym", "leqsym",
    "gtrsym", "geqsym", "lparentsym", "rparentsym", "commasym", "semicolonsym",
    "periodsym", "becomessym", "beginsym", "endsym", "ifsym", "thensym",
    "whilesym", "dosym", "callsym", "constsym", "varsym", "procsym", "writesym",
    "readsym" , "elsesym"};

//flags that will call our 3 different .c files
int lexout, parserout, vmout;

//global variables
FILE *ifp;
FILE *ofp;
int i;

//
int main(int argc, char *argv[])
{
  while (argc > 1)
  {
    if (strcmp(argv[1], "-l") == 0)
    {
      lexout = 1;
    }
    else if (strcmp(argv[1], "-a") == 0)
    {
        parserout = 1;
    }
    else if (strcmp(argv[1], "-v") == 0)
    {
        vmout = 1;
    }
    argc--;
    argv[]++;
  }
  compile();
}

void compile()
{
    //runs scanner which outputs a file with lexeme list that is read by the parser
    Scanner();
    // runs parser with input file (output from lex). writes to another output file.
    parser();
    // runs PMachine with output file from parser. writes to the final output file. 
    PMachine();

    if(lexout == 1)
    {
        //print out token list.
        ifp = 
        while(!feof(ifp)
        {
            fputc(fgetc())
        }
        // print out variable names in Lexeme list.
        for(i=0; i<lexemeIndex; i++)
        {
            
        }
        printf("\n\n");
        fclose(ifp);
    }
    // if no errors, print this
     if (count == 0)
     {
         printf("No errors, program is syntactically correct.\n\n");
     }
    //parse codegen 
    if(parserout == 1)
    {
        //generate code and print it
        ifp = 
        while((i=fgetc(ifp)) != EOF)
        {
            printf("%c", i);
        }
        printf("\n");
        fclose(ifp);
    }
    if(vmout == 1)
    {
        ifp = 
        while((i=fgetc(ifp)) != EOF)
        {
            printf("%c", i);
        }
        printf("\n");
        fclose(ifp);
    }
}
