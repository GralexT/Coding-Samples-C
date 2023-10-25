#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	//CHECK NUMBER OF COMMAND LINE ARGUMENTS
	if(argc!=2){
		fprintf(stderr,"Usage: %s value1",argv[0]);
		exit(EXIT_FAILURE);
	}
	//IDENTIFY IF YEAR IS LEAP YEAR(TRUE) OR NOT(FALSE)
	else{
	int year = atoi(argv[1]);
	if(year%400==0 || (year%4==0 && year%100!=0)){printf("True\n");}
	else{printf("False\n");}
	return 0;
}
}
