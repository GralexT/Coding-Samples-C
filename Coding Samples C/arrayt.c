#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//int main()
//{
//	printf("Simulate Process:\n");
//	char *process[5];
//	char var = "initial";
//	strcpy(var, "changed");
//
//	strcpt(process[0], var);
//	process[1] = "test2";
//	process[2] = "test3";
//	process[3] = "test4";
//	process[1] = "testchange";
//	printf("\narr 1: %s\n, arr2: %s\n, arr3: %s\n, arr4: %s\n",process[0],process[1],process[2],process[3]);
//}

int main()
{
int temp[5];
int arr1[5] = {2,4,6,8,9};
int arr2[5] = {1,2,3,4,5};
printf("First in arr1: %i",arr1[0]);
printf("First in arr2: %i",arr2[0]);
temp = arr1;
arr1 = arr2;
arr2 = temp;
printf("First in arr1: %i",arr1[0]);
printf("First in arr2: %i",arr2[0]);
}
