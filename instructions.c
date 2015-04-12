/* 
 * 	Title: instructions.c 
 * 	Name: Nga Pham, Sartrapat Saengcharoentrakul 
 * 	Username: npham02, ssaeng01
 * 	Assigment 6
 * 
 * 	Purpose: implementation for um instructions using umMem_T types
 *     		 and functions
 */

#include <stdlib.h>
#include <stdio.h>

#include "assert.h"
#include "um_memory.h"
#include "instructions.h"
#include <math.h>

const uint32_t NOT_ONE = ~0;

void conditional_move(umMem_T memory, int A, int B, int C)
{
        if (register_get(memory, C) != 0) {
                register_put(memory, A, register_get(memory, B));
        }        
}

void segmented_load(umMem_T memory, int A, int B, int C)
{
        uint32_t result = segment_get(memory, register_get(memory, B), 
                                              register_get(memory, C));
        register_put(memory, A, result);
}

void segmented_store(umMem_T memory, int A, int B, int C)
{
        segment_put(memory, register_get(memory, A), register_get(memory, B), 
                                                     register_get(memory, C));
}

void addition(umMem_T memory, int A, int B, int C)
{     
        uint32_t result = (register_get(memory, B) + register_get(memory, C)); 
        register_put(memory, A , result);
}

void multiplication(umMem_T memory, int A, int B, int C)
{
        uint32_t result = (register_get(memory, B) * register_get(memory, C));
        register_put(memory, A, result);               
}

void division(umMem_T memory, int A, int B, int C)
{
        assert(register_get(memory, C) != 0);
        uint32_t result = (register_get(memory, B) / register_get(memory, C));
        register_put(memory, A, result);
}

void bitwise_NAND(umMem_T memory, int A, int B, int C)
{
        uint32_t value = ~(register_get(memory, B) & register_get(memory, C));
        register_put(memory, A, value);
}

void halt(umMem_T memory)
{
	program_free(&memory);
        exit(0);
}

void map_segment(umMem_T memory, int B, int C)
{
        segment_map(memory, B, register_get(memory, C));
}

void unmap_segment(umMem_T memory, int C)
{
        assert(register_get(memory, C) != 0);
        segment_unmap(memory, register_get(memory, C));
}

void output(umMem_T memory, int C)
{
        uint32_t toPrint = register_get(memory, C);
        assert(toPrint <= 255);
        putchar((int)toPrint);
}

void input(umMem_T memory, int C)
{
        int input = getc(stdin);
        //assert(input >= 0 && input <= 255);
        if (input != EOF) {
                register_put(memory, C, (uint32_t)input);
        } else { 
                register_put(memory, C, NOT_ONE);
        }
}

uint32_t load_program(umMem_T memory, int B, int C)
{
        uint32_t rB_value = register_get(memory, B);

        /* $[rB] is duplicated */
        if (rB_value != 0) {
                segment_unmap(memory, 0);
                int length = segment_length(memory, rB_value);
                segment_map(memory, 0, length);
                
                /* copy from segment with ID $r[B] to segment 0 */
                for (int i = 0; i < length; i++) {
                        uint32_t value = segment_get(memory, rB_value, i);
                        segment_put(memory, 0, i, value);
                }
        }
        return register_get(memory, C);
}

void load_value(umMem_T memory, unsigned A, unsigned value)
{
        register_put(memory, A, (uint32_t)value);
}
