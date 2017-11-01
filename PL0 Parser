#include <stdlib.h>

#include <stdio.h>

#include <string.h>

#include <ctype.h>

#include <math.h>



#define MAX_SYMBOL_TABLE_SIZE 1000

#define CODE_SIZE 1000

#define MAX_LEX_LEVELS 3

#define MAX_CHAR 11

#define MAX_DIGIT 5



typedef enum

{

   nulsym = 1, identsym, numbersym, plussym, minussym,

   multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,

   gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,

   periodsym, becomessym, beginsym, endsym, ifsym, thensym,

   whilesym, dosym, callsym, constsym, varsym, procsym, writesym,

   readsym , elsesym

} token_type;



//Contains all tokens

typedef struct

{

    token_type tokenID;

    int numval;

    char name[12];

}tokenStruct;



//Data structure for the symbol

typedef struct

    {

	       int kind; 		// const = 1, var = 2, proc = 3

	       char name[10];	// name up to 11 chars

	       int val; 		// number (ASCII value)

	       int level; 		// L level

	       int addr; 		// M address

    }symbol;



typedef struct

{

    int op; //Opcode

    int l; //L

    int m; //M

}instruction;



symbol_table[MAX_SYMBOL_TABLE_SIZE]  = {0};



/* Constants stored by kind, name, and value.

   Variables stored by kind, name, L and M.

   Procedures stored by kind, name, L and M.

*/



//Global Variables

int token, num, kind, count = 0;



//functions

void errors(int errorCases);

void parser(FILE *ifp, symbol *table, instruction *code);









int main(int argc, char *argv[])

{

    FILE *ifp = fopen("output.txt","r");

    FILE *ofp = fopen("parseroutput.txt", "w");

    int i, lexlev = 0, dataindex = 0, tableindex = 0;



    instruction code[CODE_SIZE];



    //calls function parser that runs program

    parser(ifp, table, code);

    for(i=0; i< ;i++)

    {



    }

    fclose(ifp);

    fclose(ofp);



}



void parser(FILE *ifp, symbol *table, instruction *code)

{



}



void errors(int errorCases)

{

    count++;



    switch (errorCases)

    {

        case 1:

            printf("Error 1: Use = instead of :=.\n");

            break;

        case 2

            printf("Error 2: = must be followed by a number.\n")

        case 3:

            printf("Error 3: Identifier must be followed by =.\n");

            break;

        case 4:

            printf("Error 4: const, var, procedure must be followed by identifier.\n");

            break;

        case 5:

            printf("Error 5: Semicolon or comma missing.\n");

            break;

        case 6:

            printf("Error 6: Incorrect symbol after procedure declaration.\n");

            break;

        case 7:

            printf("Error 7: Statement expected.\n");

            break;

        case 8:

            printf("Error 8: Incorrect symbol after statement part in block.\n");

            break;

        case 9:

            printf("Error 9: Period expected.\n");

            break;

        case 10:

            printf("Error 10: Semicolon between statements missing.\n");

            break;

        case 11:

            printf("Error 11: Undeclared identifier.\n");

            break;

        case 12:

            printf("Error 12: Assignment to constant or procedure is not allowed.\n");

            break;

        case 13:

            printf("Error 13: Assignment operator expected.\n");

            break;

        case 14:

            printf("Error 14: call must be followed by an identifier.\n");

            break;

        case 15:

            printf("Error 15: Call of a constant or variable is meaningless.\n");

            break;

        case 16:

            printf("Error 16: then expected.\n");

            break;

        case 17:

            printf("Error 17: Semicolon or } expected.\n");

            break;

        case 18:

            printf("Error 18: do expected.\n");

            break;

        case 19:

            printf("Error 19: Incorrect symbol following statement.\n");

            break;

        case 20:

            printf("Error 20: Relational operator expected.\n");

            break;

        case 21:

            printf("Error 21: Expression must not contain a procedure identifier");

            break;

        case 22:

            printf("Error 22: Right parenthesis missing.\n");

            break;

        case 23:

            printf("Error 23: The preceding factor cannot begin with this symbol.\n");

            break;

        case 24:

            printf("Error 24: An expression cannot begin with this symbol.\n");

            break;

        case 25:

            printf("Error 25: This number is too large.\n");

            break;

        case 26:

            printf("Error 26: Level is larger than the maximum allowed lexicographical levels.\n");

            break;

        default:

            break;

    }

    exit(1);

}

