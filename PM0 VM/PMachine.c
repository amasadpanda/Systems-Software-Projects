#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

typedef struct
{
	int op;
	int r;
	int l;
	int m;
} instruction;

const char* INST_NAME[23] = {"LIT", "RTN", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SIO"
									, "NEG", "ADD", "SUB", "MUL", "DIV", "ODD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};

int REGISTERS[8];
int STACK[MAX_STACK_HEIGHT] = {0};
instruction CODE[MAX_CODE_LENGTH];
int RUN = 0;

instruction IR;
int BP, SP, PC = 0;

int base(int l, int base)
{
	int b1;
	b1 = base;
	while(l > 0)
	{
		b1 = STACK[b1+1];
		l--;
	}
	return b1;
}

int execute(instruction exec)
{
	switch(exec.op)
	{
		// LIT, literal value to register
		case 1: REGISTERS[exec.r] = exec.m;
			break;
		// RTN, returns from function
		case 2: SP = BP-1;
				  BP = STACK[SP+3];
				  PC = STACK[SP+4];
			break;
		// LOD, loads values from STACK to register
		case 3: REGISTERS[exec.r] = STACK[base(exec.l, BP) + exec.m];
			break;
		// STO, stores from register to STACK
		case 4: STACK[base(exec.l, BP) + exec.m] = REGISTERS[exec.r];
			break;
		// CAL, puts a new AR onto the STACK
		case 5: STACK[SP+1] = 0;
				  STACK[SP+2] = base(exec.l, BP);
				  STACK[SP+3] = BP;
				  STACK[SP+4] = PC;
				  BP = SP+1;
				  PC = exec.m;
			break;
		// INC, increments SP
		case 6: SP = SP + exec.m;
			break;
		// JMP, jump to instruction in CODE
		case 7: PC = exec.m;
			break;
		// JPC, jump if R=0
		case 8: if(REGISTERS[exec.r] == 0)
					{
						PC = exec.m;
					}
			break;
		// SIO, print out REGISTERS, red in REGISTERS, set halt flag
		case 9: if(exec.m == 1)
					printf("%d\n", REGISTERS[exec.r]);
				  else if(exec.m == 2)
				  	scanf("%d", &REGISTERS[exec.r]);
				  else if(exec.m == 3)
				  	RUN = 0;
			break;
		// NEG
		case 10: REGISTERS[exec.r] = 0 - REGISTERS[exec.l];
			break;
		// ADD
		case 11: REGISTERS[exec.r] = REGISTERS[exec.l] + REGISTERS[exec.m];
			break;
		// SUB
		case 12: REGISTERS[exec.r] = REGISTERS[exec.l] - REGISTERS[exec.m];
			break;
		// MUL
		case 13: REGISTERS[exec.r] = REGISTERS[exec.l] * REGISTERS[exec.m];
			break;
		// DIV
		case 14: REGISTERS[exec.r] = REGISTERS[exec.l] / REGISTERS[exec.m];
			break;
		// ODD
		case 15: REGISTERS[exec.r] = REGISTERS[exec.r] % 2;
			break;
		// MOD
		case 16: REGISTERS[exec.r] = REGISTERS[exec.l] % REGISTERS[exec.m];
			break;
		// EQL
		case 17: REGISTERS[exec.r] = REGISTERS[exec.l] == REGISTERS[exec.m];
			break;
		// NEQ
		case 18: REGISTERS[exec.r] = REGISTERS[exec.l] != REGISTERS[exec.m];
			break;
		// LSS
		case 19: REGISTERS[exec.r] = REGISTERS[exec.l] < REGISTERS[exec.m];
			break;
		// LEQ
		case 20: REGISTERS[exec.r] = REGISTERS[exec.l] <= REGISTERS[exec.m];
			break;
		// GTR
		case 21: REGISTERS[exec.r] = REGISTERS[exec.l] > REGISTERS[exec.m];
			break;
		// GEQ
		case 22: REGISTERS[exec.r] = REGISTERS[exec.l] >= REGISTERS[exec.m];
			break;
	}
}

void printInstruct(instruction instr, int codeLine)
{
	printf("%d\t%s %d\t%d\t%d\t%d\t%d\t%d\n", codeLine, INST_NAME[instr.op], instr.r, instr.l, instr.m, PC, BP, SP);

}

void printCode()
{
	int i = 0;
	printf("Program Printout\n\n");
	while(CODE[i].op != 0)
	{
		printf("  %d %s %d %d %d\n", i, INST_NAME[CODE[i].op], CODE[i].r, CODE[i].l, CODE[i].m);
		i++;
	}
	printf("\n");
}

int main(int argc, char *argv[])
{

	/*FILE *ifp = NULL;
	int *buffer = NULL
	size_t len;
	int count = 0;

	ifp = fopen(argv[1],"r");

	if (ifp == NULL)
	{
			fprintf(stderr, "File %s not found.\n", argv[1]);
			exit(1);
	}
	else
	{
			//prints header first before pulling data out of txt file
			printf("Factorial Program:\n");
			while(!feof(ifp))
			{
					fscanf(ifp,"%d",&code[count].op);
					fscanf(ifp, "%d", &code[count].r)
					fscanf(ifp,"%d", &code[count].l)
					fscanf(ifp, "%d", &code[count].m));
					count++;
			}
	}*/


	RUN = 1;
	CODE[0].op = 1; CODE[0].r = 0; CODE[0].m = 123;
	CODE[1].op = 9; CODE[1].r = 0; CODE[1].m = 1;
	CODE[2].op = 9; CODE[2].r = 1; CODE[2].m = 2;
	CODE[3].op = 9; CODE[3].r = 1; CODE[3].m = 1;
	CODE[4].op = 9; CODE[4].r = 0; CODE[3].m = 3;

	printCode();
	printf("Program Stack Trace\nInstructions\t\t\tPC\tBP\tSP\n");
	while(RUN && PC < MAX_CODE_LENGTH)
	{
		IR = CODE[PC];
		PC++;

		printInstruct(IR, PC-1);
		execute(IR);

	}

}
