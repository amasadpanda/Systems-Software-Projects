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
    nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
    multsym = 6,  slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
    gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
    periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
    whilesym = 25, dosym =26, callsym =27, constsym = 28, varsym = 29, procsym=30, writesym = 31,
    readsym = 32, elsesym = 33, colonsym = 34
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
