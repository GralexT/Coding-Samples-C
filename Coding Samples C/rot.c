#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Compile this program with:
//      cc -std=c99 -Wall -Werror -pedantic -o rot rot.c

#define ROT 3

//  The rotate function returns the character ROT positions further along the
//  alphabetic character sequence from c, or c if c is not lower-case

char rotate(char c)
{
        // Check if c is lower-case or not
        if (islower(c))
        {
                // The ciphered character is ROT positions beyond c,
                // allowing for wrap-around
                return ('a' + (c - 'a' + ROT) % 26);
        }
	else if (isupper(c)){return ('A' + (c - 'A' + ROT) % 26);}
        else
        {
                return c;
        }
}

//  Execution of the whole program begins at the main function

int main(int argc, char *argv[])
{
        // Exit with an error if the number of arguments (including
        // the name of the executable) is not precisely 2
        if(argc != 2)
        {
                fprintf(stderr, "%s: program expected 1 argument, but instead received %d\n", argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
                // Calculate the length of the first argument
                int length = strlen(argv[1]);

                // Loop for every character in the text
                for(int i = 0; i < length; i++)
                {
                        // Determine and print the ciphered character
                        printf("Character Position: %i, Output: %c, Input: %c\n",i, rotate(argv[1][i]),argv[1][i]);
                }

                // Print one final new-line character
                printf("\n");

                // Exit indicating success
                exit(EXIT_SUCCESS);
        }
        return 0;
}
