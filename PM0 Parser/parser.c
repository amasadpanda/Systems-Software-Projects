#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pl0data.h.

#define CODE_SIZE 1000
#define MAX_CHAR 11
#define MAX_DIGIT 5
#define MAX_LEX_LEVELS 3
#define MAX_SYMBOL_TABLE_SIZE 1000

//Data structure for the symbol
typedef struct
{
  int kind; 	// const = 1, var = 2, proc = 3
  char name[10];	// name up to 11 chars
  int val; // number (ASCII value)
  int level; // L level
  int addr; // M address
}symbol;

instruction CODE_TABLE[CODE_SIZE];
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
/* Constants stored by kind, name, and value.
   Variables stored by kind, name, L and M.
   Procedures stored by kind, name, L and M.
*/

//Global Variables
int TOKEN, NUM, KIND, CODEINDEX = 0, SYMBOLCOUNT = 0, LEXEMEINDEX = 0, DATAINDEX = 4;
char ID[12];
tokenStruct lexemeList;

//functions
void errors(int errorCases);
void parser(FILE *ifp, symbol *table, instruction *code);
void gen(int op, int l, int m, instruction *code);
int nextToken(FILE *ifp);
int getToken(FILE *ifp);

// Gets the list of lexeme from a file, stores the tokens and returns how many tokens are found.
int lexemeList(File* lexemeFile)
{
  lexemeList = malloc(sizeof(tokenStruct) * 300);
  int count = 0;
  while(!feof(lexemFile))
  {
    fscanf(lexemeFile, "%d", &lexemeList[count].tokenID);
    if(lexemeList[count].tokenID == 2)
      fscanf(lexemeFile, "%s", &lexemeList[count].name);
    else if(lexemeList[count].tokenID == 3)
      fscanf(lexemeFile, "%d", &lex1emeList[count].numval);
    count++;
  }

  return count;
}

tokenStruct getToken()
{
  TOKEN = lexemeList[LEXEMEINDEX].tokenID;
	if (token == 2)
	{
	   strcpy(ID, lexemeList[++LEXEMEINDEX].name);
	}
	else if (token == 3)
	{
      NUM = lexemeList[++LEXEMEINDEX].numval;
	}
	LEXEMEINDEX++;
	return token;
}

int relationalSymbol()
{
  switch(TOKEN)
  {
    case token_type.eqsym: return 1;
    case token_type.neqsym: return 1;
    case token_type.lessym: return 1;
    case token_type.leqsym: return 1;
    case token_type.gtrsym: return 1;
    case token_type.geqsym: return 1;
    default: return 0;
  }
  return 0;
}

void generateVar()
{
  symbol_table[SYMBOLCOUNT].kind = 2;
  // is this allowed
  strcpy(symbol_table[SYMBOLCOUNT].name, NAME);
  symbol_table[SYMBOLCOUNT].level = 0;
  symbol_table[SYMBOLCOUNT].addr = DATAINDEX;

  SYMBOLCOUNT++;
  DATAINDEX++;
}

void generateNum()
{
  symbol_table[SYMBOLCOUNT].kind = 3;
  symbol_table[SYMBOLCOUNT].value = NUM;
  symbol_table[SYMBOLCOUNT].level = 0;
  symbol_table[SYMBOLCOUNT].addr = DATAINDEX;

  SYMBOLCOUNT++;
  DATAINDEX++;
}

void generateCode(int opcode, int r, int l, int m)
{
  if(CODEINDEX > CODE_SIZE)
    error(27);

  CODE_TABLE[CODEINDEX].op = opcode;
  CODE_TABLE[CODEINDEX].r = r;
  CODE_TABLE[CODEINDEX].l = l;
  CODE_TABLE[CODEINDEX].m = m;

  CODEINDEX++;

}

int findVar()
{
  int i;
  for(i = 0; i < SYMBOLCOUNT; i++)
  {
    if(strcmp(symbol_table[i].name, NAME))
      return i;
  }
  return -1;
}

/*
##############################################
  Below is the code pertaining to the parser
##############################################
*/
void factor()
{
  if(TOKEN == token_type.identsym)
  {
    if(findVar() < 0)
      error();
    getToken();
  }
  else if(TOKEN == token_type.numbersym)
  {
    getToken();
  }
  else if(TOKEN == token_type.lparentsym)
  {
    getToken();
    expression();
    if(TOKEN != token_type.rparentsym)
      error();
    getToken();
  }
  else
  {
    error();
  }
}

void term();
{
  factor();
  while(TOKEN == token_type.multsym || TOKEN == token_type.slashsym)
  {
    getToken();
    factor();
  }
}

void expression()
{
  if(TOKEN == token_type.plussym || TOKEN == token_type.minussym)
    getToken();
  term();
  while(TOKEN == token_type.plussym || TOKEN == token_type.minussym)
  {
    getToken();
    term();
  }
}

void condition()
{
  if(TOKEN == token_type.oddsym)
  {
    getToken();
    expression();
  }
  else
  {
    expression();
    if(!relationalSymbol)
      error();
    getToken();
    expression();
  }
}

void statement()
{
  if(TOKEN == token_type.identsym)
  {
    int place = findVar();
    getToken();
    if(TOKEN != token_type.becomessym)
      error();
    getToken();
    expression();
    generateCode(4, 1, 0, symbol_table[place].addr);
  }
  else if(TOKEN == token_type.beginsym)
  {
    getToken();
    statement();
    while(TOKEN == token_type.semicolonsym)
    {
      getToken();
      statement();
    }
    if(TOKEN != token_type.endsym)
      error();
    getToken();
  }
  else if(TOKEN == token_type.ifsym)
  {
    getToken();
    condition();
    int f = CODEINDEX;
    generateCode(8, 0, 0, CODEINDEX);
    if(TOKEN != token_type.thensym)
      error();
    getToken();
    statement();
    CODE_TABLE[f].m = CODEINDEX;
  }
  else if(TOKEN == token_type.whilesym)
  {
    getToken();
    condition();
    int f = CODEINDEX;
    generateCode(8, 0, 0, CODEINDEX);
    if(TOKEN != token_type.dosym)
      error();
    getToken();
    statement();
    CODE_TABLE[f].m = CODEINDEX;
  }
}

void block()
{
  if(TOKEN == token_type.constsym)
  {
    while(true)
    {
      getToken();
      if(TOKEN != token_type.identsym)
        error();

      int placeInTable = findVar();
      // cannot be found
      if(placeInTable < 0)
        error();

      getToken();
      if(TOKEN != token_type.eqsym)
        error();
      getToken();
      if(TOKEN != token_type.numbersym)
        error();
      symbol_table[placeInTable] = NUM;
      getToken();
      if(TOKEN != token_type.commasym)
        break;
    }
    if(TOKEN != token_type.semicolonsym)
      error();
    getToken();
  }
  if(TOKEN == token_type.varsym)
  {
    while(true)
    {
      getToken();
      if(TOKEN != token_type.identsym)
        error();
      generateVar();
      getToken();
      if(TOKEN != token_type.commasym)
        break;
    }
    if(TOKEN != token_type.semicolonsym)
      error();
    getToken();
  }
  statement();
}

void program()
{
  getToken()
  block();
  if(TOKEN != token_type.periodsym)
    error(9);
}

void parser(FILE *ifp, symbol *table, instruction *code)
{
   TOKEN = getToken(ifp);
}

void main(int n, char** args)
{
    FILE* ifp = fopen("output","r");
    FILE* ofp = fopen("assembly", "w");

    for(int i = 0; i < n; i++)
      printf("%s", args[i]);

    int i, lexlev = 0, dataindex = 0, tableindex = 0;

    instruction code[CODE_SIZE];

    //calls function parser that runs program
    parser(ifp, table, code);
    for(i=0; i<CODEINDEX; i++)
    {
	     fprintf(ofp, "%d %d %d\n", code[i].op, code[i].l, code[i].m);
    }
    fclose(ifp);
    fclose(ofp);
}

void error(int errorCase)
{
    switch (errorCases)
    {
        case 1:
            printf("Error 1: Use = instead of :=.\n");
            break;
        case 2:
            printf("Error 2: = must be followed by a number.\n");
	          break;
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
        case 27:
            printf("Error 27: Generating too much code!\n");
            break;
        default:
            break;
    }
    exit(1);
}
