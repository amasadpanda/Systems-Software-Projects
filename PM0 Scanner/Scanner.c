#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 11
#define MAX_DIGIT 5
#define MAX_TABLE_SIZE 100

const char* RESERVED[] = {"const", "var", "procedure", "call", "begin", "end",
								"if", "then", "else", "while", "do", "read","write", "null", "odd"};
const int IDs[] = {28, 29, 30, 27, 21, 22, 23, 24, 33, 25, 26, 32, 31, 1, 8};
const char SPECIAL[] = {'+', '-', '*', '/', '(', ')', '=', ',' , '.', '<', '>', ';' , ':'};

// just a switch statement to return the number of each symbol
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

// loops through the SPECIAL array to see if any character is SPECIAL
// returns the index that the match is found, or -1 otherwise
// in the case that a := (the only special character with character length 2), it is manually checked before this function
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

// loops through RESERVED words to find if the word is RESERVED
// return the index at which it is found, or -1 otherwise
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
      // The current lexeme is stored as a buffer for us to read in character one at a time
      char buffer[MAX_CHAR];
      // memset to set all things to null, so any words smaller than MAX_CHAR still terminates correctly
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

        // Below is output things
        if(place >= 0)
        {
          fprintf(ofp, "%d ", IDs[place]);              // output
          fprintf(lfp, "%-13s%d\n", buffer, IDs[place]);// lexemes
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

      else if(isgraph(token))
      {
        // This section is reserved for characters not understood by the
        // Scanner, or characters that do not belong to PL0
        printf("[%c] is not a valid symbol.\n", token);
        return 1;
      }
      token = next;
	}
  // Outputting to stdout is done at the very end, to ensure that
  // no errors were present first
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

  printf("\n");

  fclose(ifp);
  fclose(ofp);
  fclose(lfp);

}
