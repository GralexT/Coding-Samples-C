#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void left(int size)
{
	for(int i=0;i<size;i++){printf("%*s\n",i,0);}
}

//void right(int size)
//{
//	for(int i=0;i<size;i++){printf(" "*(i-1),"*"*i,"\n");}
//}

//void leftInverted(int size)
//{
//	for(int i=0;i<size;i++){printf("*"*(size-i),"\n");}
//}

//void full(int size)
//{
//	for(int i=0;i<size;i++){;}
//}

int main(int argc,char *argv[])
{
	if(strcmp(argv[1],"left") == 0){left(atoi(argv[2]));}
	return 0;
}
