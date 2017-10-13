#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR 11
#define MAX_DIGIT 5
#define MAX_TABLE_SIZE 37

typedef enum
{
    nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
    multsym = 6,  slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
    gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
    periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
    whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
    readsym = 32, elsesym = 33
} token_type;

typedef struct
{
	token_type kind;
	char name[MAX_CHAR];
	char number[MAX_DIGIT];
} namerecord_t;

const char* RESERVED[] = {"const", "var", "procedure", "call", "begin", "end", 
								"if", "then", "else", "while", "do", "read","write"};
const char SPECIAL[] = {'+', '-', '*', '/', '(', ')', '=', ',' , '.', '<', '>', ';' , ':'};

namerecord_t SYMBOL_TABLE[MAX_TABLE_SIZE];

int isspecial(char c)
{
	int i;
	for(i = 0; i < 13; i++)
	{
		if(c == SPECIAL[i])
			return 1;
	}
	return 0;
}

int parseWord(char c)
{
	char ident[MAX_CHAR];
	char next;
	int count;
}

int main(int argc, char *argv[])
{

	FILE *ifp = NULL;
	FILE *ofp = NULL;
	int *buffer = NULL;
	size_t len;
	
	char token;
	char next;

	if(argc < 2)
	{
		fprintf(stderr, "Correct syntax for this program: Scanner <filename>\n");
		exit(1);
	}
	
	ifp = fopen(argv[1],"r");
	if(ifp == NULL)
	{
		fprintf(stderr, "File %s not found.\n", argv[1]);
		exit(1);
	}
	else
	{
		printf("Scanning: %s\n", argv[1]);
	}
	
	
	while(!feof(ifp))
	{
			token = fgetc(ifp);
			printf("%c", token);
			
			if(isalpha(token))
			{
			
			}else if(isdigit(token))
			{
			
			}else if(isspecial(token))
			{
			
			}
	}
}
