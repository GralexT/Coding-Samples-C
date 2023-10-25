#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	if(argc < 2)
	{	
		fprintf(stderr,"Argument Usage: %s value1 value2 value3\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	else
	{
		int value = atoi(argv[1]);
		for(int i = 1; i < argc; i++){ if(value<atoi(argv[i])){value=atoi(argv[i]); }}
		printf("Maximum of three values is: %i\n",value);
		return 0;
	}
}
