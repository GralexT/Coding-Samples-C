#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//int n;
//int main(void){
//for (int i=0; i<7; i++){
//	n = i%3;
//	printf("%i mod 3 = %i\n",i,n);
//}
//}


int n = 0;
int main(void){
for (int i=0; i<10; i++){
	n = (n+1)%3;
	printf("%i",n);
}
}
