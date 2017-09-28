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

int registers[8];
int stack[MAX_STACK_HEIGHT] = {0};
instruction code[MAX_CODE_LENGTH];

instruction IR;
int BP, SP, PC = 0;

int base(int l, int base)
{
	int b1;
	b1 = base;
	while(l > 0)
	{
		b1 = stack[b1+1];
		l--;
	}
	return b1;
}

int execute(instruction exec)
{
	switch(exec.op)
	{
		// LIT, literal value to register
		case 1: registers[exec.r] = exec.m;
			break;
		// RTN, returns from function
		case 2: SP = BP-1;
				  BP = stack[SP+3];
				  PC = stack[SP+4];
			break;
		// LOD, loads values from stack to register
		case 3: registers[exec.r] = stack[base(exec.l, BP) + exec.m];
			break;
		// STO, stores from register to stack
		case 4: stack[base(exec.l, BP) + exec.m] = registers[exec.r];
			break;
		// CAL, puts a new AR onto the stack
		case 5: stack[SP+1] = 0;
				  stack[SP+2] = base(exec.l, BP);
				  stack[SP+3] = BP;
				  stack[SP+4] = PC;
				  BP = SP+1;
				  PC = exec.m;
			break;
		// INC, increments SP
		case 6: SP = SP + exec.m;
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			break;
		case 16:
			break;
		case 17:
			break;
		case 18:
			break;
		case 19:
			break;
		case 20:
			break;
		case 21:
			break;
	}
}

int main(int argc, char *argv[])
{
	

	
	int run = 1;
	
	while(run && PC < MAX_CODE_LENGTH)
	{	
		IR = code[PC];
		PC++;
		
		execute(IR);
	}	
		
}