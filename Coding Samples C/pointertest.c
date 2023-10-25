#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int main(int argcount, char *argvalue[]){
int total;
int *p     = &total ;
int bigger;

    total  =  8;

    printf("value of variable total is:       %i\n",   total );
    printf("value pointed to by pointer p is: %i\n\n", *p );

    *p  =  *p + 2 ;     // increment, by 2, the value that p points to   

    printf("value of variable total is:       %i\n",   total );
    printf("value pointed to by pointer p is: %i\n\n", *p );

    bigger  =  *p + 2 ; // just fetch the value that p points to   

    printf("value of variable total is:       %i\n", total );
    printf("value of variable bigger is:      %i\n", bigger );
    printf("value pointed to by pointer p is: %i\n", *p );

exit(EXIT_SUCCESS);
}
