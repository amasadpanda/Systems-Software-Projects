#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pl0data.h>

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
int sp, max_level, current_level, reg_ptr;
tokenStruct lexemeList;

tokenStruct current_token;

//functions
void error(int errorCase);
void parser(FILE *ifp, symbol *table, instruction *code);
void gen(int op, int l, int m, instruction *code);
int nextToken(FILE *ifp);
int getToken(FILE *ifp);
void add_to_symbol_table( int k, char name[], int val, int addr ); //Inserts a symbol to the symbol table.

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

  int i; //index.

  //Boolean to check if identifier is declared. If so, = 1; else, = 0.
  int declared = 0;

  //Save symbol table index for found identifier.
  int ident_index;

    //ident
  if(TOKEN != token_type.identsym)
  {

    //Check if it's in the symbol table.
    for(i = SYMBOLCOUNT - 1; i >= 0; i--)
        if(!strcmp(current_token.value, symbol_table[i].name ))
        {

            if(symbol_table[i].kind == 1 ) error(12);

            else if(symbol_table[i].kind == 2  )
            {
                declared = 1;
                ident_index = i;
            }

        }

    //Undeclared identifier.
    if(!declared ) error(11);

    getToken();

    if(strcmp(current_token.type, "20") != 0) error(3);

    getToken();

    expression();

    //store at necessary mem address
    gen(4, reg_ptr - 1, current_level - symbol_table[ ident_index ].level, symbol_table[ ident_index ].addr - 1  );


    reg_ptr--;

  }

  //"call" ident
    else if(!strcmp(current_token.type, "27"))
    {

        int declared = 0;

        getToken();

        //identifier expected.
        if(strcmp(current_token.type, "2")) error(14);

        //Check if identifier has been declared.
        for( i = SYMBOLCOUNT - 1; i >= 0; i-- )
            if(!strcmp( current_token.value, symbol_table[i].name))
            { //Found!
                ident_index = i; //Save identifier index.
                declared = 1;
            }

        if(!declared) 
        { 
            error(11);
        }

        if(symbol_table[ident_index].kind == 3)
        {
            gen(5, 0, level, symbol_table[ident_index].addr);
            current_level++;
        }
        else
            error(14); //Call must be followed by a procedure identifier.

        getToken();

    }


   //begin
  else if(strcmp(current_token.type, "21") )
  {

    getToken();

    statement();

    while(!strcmp(current_token.type, "18") )
    {

      getToken();

      statement();

    }

      getToken();

  }

  // if (cond) -> stmt
  else if( !strcmp(current_token.type, "23") )
  {

    getToken();

    condition();

    if( strcmp(current_token.type, "24" ) != 0 ) error(16);

    getToken();

    //start code gen.

    int ctemp = CODEINDEX; //Save current code index.


     gen(8, reg_ptr - 1, 0, 0 );


    statement();

    getToken();

    //else
    if(!strcmp(current_token.type, "33")){
        //current cx
        int ctemp2 = CODEINDEX;

        gen(7, 0, 0, 0);

        CODE_TABLE[ctemp].m = CODEINDEX;
        getToken();

        statement();

        CODE_TABLE[ctemp2].m = CODEINDEX;
        reg_ptr--;

    }

    else{
        CODE_TABLE[ ctemp ].m = cx;

        reg_ptr--;
    }
  }

  // while(cond) do (stmt)
  else if( !strcmp(current_token.type, "25") ){

    int cx1 = CODEINDEX;

    get_next_token();

    condition();

    int cx2 = CODEINDEX;

    gen( 8 , reg_ptr - 1, 0, 0 );

    if( strcmp(current_token.type, "26" ) != 0 ) error(18);

    getToken();

    statement();

    gen( 7 , 0, 0, cx1 );

    CODE_TABLE[ cx2 ].m = CODEINDEX;

    reg_ptr--;

  }

  //read ident
  else if( !strcmp( current_token.type, "32" ) ){

    getToken();

    if( strcmp( current_token.type, "2") != 0 ) error(29);

    //Check if identifier is in symbol table.
    for( i = SYMBOLCOUNT - 1; i >= 0; i-- )
        if( !strcmp( current_token.value, symbol_table[i].name ) ){

            declared = 1;
            ident_index = i;

        }

    //Undeclared ident.
    if( !declared ) error(11);

    //take user input for register.
     gen( 10 , reg_ptr, 0, 2);

     //Read into var.
     if( symbol_table[ ident_index ].kind == 2)
        gen( 4, reg_ptr, current_level - symbol_table[ ident_index ].level, symbol_table[ ident_index ].addr - 1 ); //Register to memory.

     //error if read into const
     else if( symbol_table[ ident_index ].kind == 1 )
        error( 12 );

     getToken();

  }

  // write ident
  else if( !strcmp( current_token.type, "31") ){

    getToken();

    if( strcmp( current_token.type, "2") != 0 ) error(29);

    //Check if ident is in symbol table.
    for( i = SYMBOLCOUNT - 1; i >= 0; i-- )
        if( !strcmp( current_token.value, symbol_table[i].name ) ){

            if( symbol_table[i].kind == 1 || symbol_table[i].kind == 2  ){
                declared = 1;
                ident_index = i;
            }

        }

    //Undeclared ident.
    if( !declared ) error(11);



     //Get var from main mem.
     if( symbol_table[ ident_index ].kind == 2){
        gen( 3 , reg_ptr, current_level - symbol_table[ ident_index ].level, symbol_table[ ident_index ].addr - 1 ); //Memory to register.
        gen( 9 , reg_ptr, 0, 1 ); //Register to screen.
     }

     //Get const from symbol table.
     else if( symbol_table[ ident_index ].kind == 1){
        gen( 1 , reg_ptr, 0, symbol_table[ ident_index ].val );
        gen( 9 , reg_ptr, 0, 1 );
     }

     getToken();

  }

}

void block()
{
  int val;
  int proc_place; 
  int space = 4;  
  sp = 3;	  
  int proc_index; // index for procedures
  int jump_proc; // jumps to the procedure code.
  
  gen(7, 0, 0, 0) //initialize jump 
	  
   if(TOKEN == token_type.constsym)
  {
    while(true)
    {
      getToken();
      if(TOKEN != token_type.identsym)
        error(4);

      int placeInTable = findVar();
      // cannot be found
      if(placeInTable < 0)
        error();

      getToken();
      if(TOKEN != token_type.eqsym)
        error(3);
      getToken();
      if(TOKEN != token_type.numbersym)
        error(2);
      symbol_table[placeInTable] = NUM;
      getToken();
      if(TOKEN != token_type.commasym)
        break;
    }
    if(TOKEN != token_type.semicolonsym)
      error(5);
    getToken();
  }
  if(TOKEN == token_type.varsym)
  {
    while(true)
    {
      getToken();
      if(TOKEN != token_type.identsym)
        error(4);
      generateVar();
      getToken();
      if(TOKEN != token_type.commasym)
        break;
    }
    if(TOKEN != token_type.semicolonsym)
      error(5);
    getToken();
  }
  statement();

    //procedure section
    while(TOKEN != token_type.procsym)
    {

    getToken();

    //identifier must go after procedure
    if((TOKEN == token_type.identsym) != 0 ) 
    error(4);
    strcpy(name, token_type.numval);

    //Create symbol table entry
    add_to_symbol_table( 3, name, 0, 0 );
    proc_index = SYMBOLCOUNT - 1; //Procedure index.
    symbol_table[proc_index].level = level;
    symbol_table[proc_index].addr = jump_proc + 1;
    getToken();
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
    switch (errorCase)
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
void add_to_symbol_table( int k, char name[], int val, int addr ){

  //add to symbol table.
  symbol_table[SYMBOLCOUNT].kind = k;
  strcpy(symbol_table[SYMBOLCOUNT].name, name );
  symbol_table[SYMBOLCOUNT].val = val;
  symbol_table[SYMBOLCOUNT].addr = addr;

  SYMBOLCOUNT++;

}
