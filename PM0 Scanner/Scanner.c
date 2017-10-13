#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 11
#define MAX_DIGIT 5
#define MAX_TABLE_SIZE 100

typedef enum
{
    nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
    multsym = 6,  slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33
} token_type;

const char* RESERVED[] = {"const", "var", "procedure", "call", "begin", "end",
								"if", "then", "else", "while", "do", "read","write", "null", "odd"};
const int IDs[] = {28, 29, 30, 27, 21, 22, 23, 24, 33, 25, 26, 32, 31, 1, 8};
const char SPECIAL[] = {'+', '-', '*', '/', '(', ')', '=', ',' , '.', '<', '>', ';' , ':'};

int getsym(char c)
{
  switch(c)
  {
    case '+': return 4;
    case '-': return 5;
    case '*': return 6;
    case '/': return 7;
    case '=': return 9;
    case '#': return 10;
    case '<': return 11;
    case '$': return 12;
    case '>': return 13;
    case '%': return 14;
    case '(': return 15;
    case ')': return 16;
    case ',': return 17;
    case ';': return 18;
    case '.': return 19;
    default: return  -1;
  }
}

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

int isreserved(char* word)
{
  int i;
  for(i = 0; i < 13; i++)
  {
    if(strcmp(RESERVED[i], word) == 0)
      return i;
  }
  return -1;
}

int main(int argc, char *argv[])
{

	FILE *ifp = NULL;
	FILE *ofp = NULL;
  FILE *lfp = NULL;
	int *buffer = NULL;
	size_t len;

	char token;
	char next;
  int placeh = 0;

	if(argc < 2)
	{
		fprintf(stderr, "Correct syntax for this program: scanner <filename>\n");
		exit(1);
	}

	ifp = fopen(argv[1],"r");
	if(ifp == NULL)
	{
		fprintf(stderr, "File %s not found.\n", argv[1]);
		exit(1);
	}

	printf("Scanning: %s\n", argv[1]);
  ofp = fopen("output","w");
  lfp = fopen("lexemes","w");

  token = fgetc(ifp);
	while(!feof(ifp))
	{
      char buffer[MAX_CHAR];
      memset( buffer, '\0', sizeof(char)*MAX_CHAR );
      int place = 1;
      buffer[0] = token;
      next = fgetc(ifp);

			if(isalpha(token))
			{
        while(isalnum(next))
        {
          buffer[place] = next;
          place++;

          if(place > MAX_CHAR)
          {
            printf("[%s] identifier too long. The maximum allowed length is %d\n", buffer, MAX_CHAR);
            return 1;
          }
          next = fgetc(ifp);
        }
        place = isreserved(buffer);
        if(place >= 0)
        {
          fprintf(ofp, "%d ", IDs[place]);
          fprintf(lfp, "%-13s%d\n", buffer, IDs[place]);
        }
        else
        {
          fprintf(ofp, "2 %s ", buffer);
          fprintf(lfp, "%-13s%d\n", buffer, 2);
        }
			}

      else if(isdigit(token))
			{
        while(isdigit(next))
        {
          buffer[place] = next;
          place++;

          if(place > MAX_DIGIT)
          {
            printf("[%s] number too long. The maximum allowed length is %d\n", buffer, MAX_DIGIT);
            return 1;
          }
          next = fgetc(ifp);
        }
        if(isalpha(next))
        {
          printf("[%s%c] Cannot start identifiers with a number", buffer,next);
          return 1;
        }
        fprintf(ofp, "3 %s ", buffer);
        fprintf(lfp, "%-13s3\n", buffer);
		  }

      else if(isspecial(token))
      {
        if(isspecial(next))
        {
          if(token == ':' && next == '=')
          {
            fprintf(ofp, "20 ");
            fprintf(lfp, ":=           20\n");
            next = fgetc(ifp);
          }
        }
        else
        {
          fprintf(ofp, "%d ", getsym(token));
          fprintf(lfp, "%-13c%d\n", token, getsym(token));
        }
      }

      else
      {
        // This section is reserved for characters not understood by the
        // Scanner, or characters that do not belong to PL0
      }
      token = next;
	}

  fclose(ifp);
  fclose(ofp);
  fclose(lfp);

  ifp = fopen(argv[1],"r");
  ofp = fopen("output","r");
  lfp = fopen("lexemes","r");

  printf("\n");
  while(!feof(ifp))
  {
    fputc(fgetc(ifp), stdout);
  }

  printf("\nLexeme Table:\n");
  while(!feof(lfp))
  {
    fputc(fgetc(lfp), stdout);
  }

  printf("\nLexeme List:\n");
  while(!feof(ofp))
  {
    fputc(fgetc(ofp), stdout);
  }

  fclose(ifp);
  fclose(ofp);
  fclose(lfp);

}
