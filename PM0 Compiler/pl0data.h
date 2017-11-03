#define MAX_SYMBOL_TABLE_SIZE 1000
#define CODE_SIZE 1000
#define MAX_LEXI_LEVELS 3

//variables

extern int lexout;
extern int parserout;
extern int vmout;
extern int count;

//Given enum
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym , elsesym
} token_type;

//struct used to contain all tokens details
typedef struct {
    token_type tokenID;
    int numval;
    char name[12];
}tokenStruct;

extern tokenStruct lexList[5000];
extern int LEXEMEINDEX;

 typedef struct {
     int op; //Opcode
     int l;  //L
     int m;  //M
 } instruction;

extern instruction code[CODE_SIZE];


//Functions shared throughout 3 .c files.

//found and called in main.c
void compile(void);

//called in main.c. These are the 3 functions (.c files) called during execution.
void Scanner(void);
void parser(void);
void PMachine(void);
