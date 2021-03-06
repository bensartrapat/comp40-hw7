#include <stdlib.h>
#include <stdio.h>

#include "parsing.h" 
#include <stdbool.h>
#include "except.h"
#include "assert.h"
#include <inttypes.h>
#include "bitpack.h"

const int UINT32_SIZE = 32;
const int BYTE_SIZE = 8;

void parsing_test(FILE *input);
FILE* getFile(int argc, char *argv[]);

int main(int argc, char *argv[])
{
        FILE *input = getFile(argc, argv);

        if (input == NULL) return 0;
      
        parsing_test(input);
        
        if (input == stdin) fclose(input);
}


/* from the command line, retrieve the file and return pointer to the file */
FILE* getFile(int argc, char *argv[])
{
        FILE *input;

        if (argc == 1) {
                input = stdin;
        } else if (argc > 1) {
                return NULL;
        } else {
                input = fopen (argv[1], "rb");
        }
        return input;
}


void parsing_test(FILE *input)
{
        int c, opcode, rA, rB, rC, opcode_13_rA, opcode_13_value; 
        int shift = UINT32_SIZE;
        uint64_t word = 0;
        while ((c = getc(input)) != EOF)
        {
                if (shift > 0) {
                        shift = shift - BYTE_SIZE;
                        fprintf(stderr, "shift: %d\n", shift);
                        word = Bitpack_newu(word, BYTE_SIZE, shift, 
                                                                (unsigned)c);
                }
                if (shift == 0) {
                        shift = UINT32_SIZE;
                        opcode = get_opcode((uint32_t)word);
                        rA = get_rA((uint32_t)word);
                        rB = get_rB((uint32_t)word);
                        rC = get_rC((uint32_t)word);
                        opcode_13_rA = opcode_13_get_rA((uint32_t)word);
                        opcode_13_value = opcode_13_get_value((uint32_t)word);
                        fprintf(stderr, "word: %d\n", (uint32_t)word);
                        fprintf(stderr, "opcode: %d, rA: %d, rB: %d, rC: %d, ", 
                                         opcode, rA, rB, rC);
                        fprintf(stderr, "opcode_rA: %d, opcode_value: %d\n",
                                         opcode_13_rA, opcode_13_value);
                }
        }                           
}