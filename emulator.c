/* 
 * 	Title: emulator.c 
 * 	Name: Nga Pham, Sartrapat Saengcharoentrakul 
 * 	Username: npham02, ssaeng01
 * 	Assigment 6
 * 
 * 	Purpose: implementation for emulator.h. Go through the program and 
 * 		 execute each instruction accordingly, calling functions from 
 * 		 instructions.h.
 * 				
 */


#include <stdlib.h>
#include <stdio.h>

#include "emulator.h"
#include "assert.h"
#include "parsing.h"
#include "um_memory.h"
#include "instructions.h"

static inline void call_instruction(umMem_T memory, uint32_t word);

void execute(umMem_T memory)
{
	/* loop through the whole segment zero */
        while (progCounter_get(memory) < segment_length(memory, 0)) {
        	/* execute each instruction, update progCounter */
                uint32_t word = segment_get(memory, 0, progCounter_get(memory));
                progCounter_update(memory, progCounter_get(memory) + 1);
                call_instruction(memory, word);
        }
}


/* Given an uint32_t word, call appropriate instruction function
 * to execute it. Fail if word doesn't code for a valid instruction 
 */
static inline void call_instruction(umMem_T memory, uint32_t word)
{
        int rA, rB, rC;			/* three_instruction value */
        unsigned ra, value;		/* instruction_13 value */
        int opcode = get_opcode(word);
        assert(opcode >= 0 && opcode <= 13);
        
        /* parse appropriate values according to given opcode */
        if (opcode == 13) {
                ra = opcode_13_get_rA(word);
                value = opcode_13_get_value(word);
        } else {
                rA = get_rA(word);	
                rB = get_rB(word);
                rC = get_rC(word);
        }
        
        switch(opcode) {
                case 0: conditional_move(memory, rA, rB, rC);
                	return;
                case 1: segmented_load(memory, rA, rB, rC);
                        return;
                case 2: segmented_store(memory, rA, rB, rC);
                        return;
                case 3: addition(memory, rA, rB, rC);
                        return;
                case 4: multiplication(memory, rA, rB, rC);
                        return;
                case 5: division(memory, rA, rB, rC);
                        return;
                case 6: bitwise_NAND(memory, rA, rB, rC);
                        return;
                case 7: halt(memory);
                        return;
                case 8: map_segment(memory, rB, rC);
                        return;
                case 9: unmap_segment(memory, rC);
                        return;
                case 10: output(memory, rC);
                        return;
                case 11: input(memory, rC);
                        return;
                case 12: load_program(memory, rB, rC);
                        return;
                case 13: load_value(memory, ra, value);
                        return;
        }
}