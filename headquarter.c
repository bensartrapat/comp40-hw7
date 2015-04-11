/* 
 * 	Title: headquarter.c 
 * 	Name: Nga Pham, Sartrapat Saengcharoentrakul 
 * 	Username: npham02, ssaeng01
 * 	Assigment 6
 * 
 * 	Purpose: read in input File, initialize memory using functions and
 * 		 types from um_memory.h, emulate the program
                 using functions from emulator.h
 * 
 * 	Note: program fails if input file is NULL 
 */

#include <stdlib.h>
#include <stdio.h>

#include <stdbool.h>
#include "except.h"
#include "assert.h"
#include <inttypes.h>
#include "um_memory.h"
#include "emulator.h"

int main(int argc, char *argv[])
{
        /* get the file input */
        FILE *input;
        if (argc == 1) {
                        input = stdin;
        } else if (argc > 2) {
                        input = NULL;
        } else {
                        input = fopen (argv[1], "rb");
        }
        if (input == NULL) {
        		return 0;
        }
        
        /* initialize memory and execute the um */
        umMem_T memory = program_init(input);
        if (input != stdin) {
        		fclose(input);
        }
        execute(memory);

	program_free(&memory);
}
