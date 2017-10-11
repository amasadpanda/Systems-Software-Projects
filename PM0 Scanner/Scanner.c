#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR 11
#define MAX_DIGIT 5


int main(int argc, char *argv[])
{

	FILE *ifp = NULL;
	int *buffer = NULL;
	size_t len;
	char current;

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
			while(!feof(ifp))
			{
					fscanf(ifp, "%c", &current);
					printf("%c", current);
			}
	}
}
