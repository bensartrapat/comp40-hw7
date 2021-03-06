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
/*
void conditional_move(umMem_T memory, int A, int B, int C)
{
        if (memory->registerList[C] != 0) {
                memory->registerList[A] = memory->registerList[B];
        }               
}

void segmented_load(umMem_T memory, int A, int B, int C)
{
        uint32_t* segment = *(uint32_t**)UArray_at(memory->segmentList,
                                        memory->registerList[B]);
        memory->registerList[A] = segment[memory->registerList[C]+1];
}

void segmented_store(umMem_T memory, int A, int B, int C)
{
        segment_put(memory, memory->registerList[A],memory->registerList[B],   
                    memory->registerList[C]);
}

void addition(umMem_T memory, int A, int B, int C)
{     
        memory->registerList[A] = (memory->registerList[B] +
memory->registerList[C]);
}

void multiplication(umMem_T memory, int A, int B, int C)
{
        memory->registerList[A] = (memory->registerList[B] *
memory->registerList[C]);       
}

void division(umMem_T memory, int A, int B, int C)
{
        assert(memory->registerList[C] != 0);
        memory->registerList[A] = (memory->registerList[B] /
memory->registerList[C]);
}

void bitwise_NAND(umMem_T memory, int A, int B, int C)
{
        memory->registerList[A] = ~(memory->registerList[B] &
memory->registerList[C]);
}

void halt(umMem_T memory)
{
	program_free(&memory);
        exit(0);
}

void map_segment(umMem_T memory, int B, int C)
{
        segment_map(memory, B, memory->registerList[C]);
}

void unmap_segment(umMem_T memory, int C)
{
        assert(memory->registerList[C] != 0);
        segment_unmap(memory, memory->registerList[C]);
}

void output(umMem_T memory, int C)
{
        uint32_t toPrint = memory->registerList[C];
        assert(toPrint <= 255);
        putchar((int)toPrint);
}

void input(umMem_T memory, int C)
{
        int input = getc(stdin);
        //assert(input >= 0 && input <= 255);
        if (input != EOF) {
                memory->registerList[C] = (uint32_t)input;
        } else { 
                memory->registerList[C] = NOT_ONE;
        }
}

uint32_t load_program(umMem_T memory, int B, int C)
{
        uint32_t rB_value = memory->registerList[B];

        if (rB_value != 0) {
                segment_unmap(memory, 0);
                uint32_t* segment = *(uint32_t**)UArray_at(memory->segmentList,
                                        rB_value);
                int length = segment[0];
                segment_map(memory, 0, length);
                
                 copy from segment with ID $r[B] to segment 0
                for (int i = 0; i < length; i++) {
                        uint32_t value = segment[i+1];//segment_get(memory,
// rB_value, i);
                        segment_put(memory, 0, i, value);
                }
        }
        return memory->registerList[C];
}

void load_value(umMem_T memory, unsigned A, unsigned value)
{
        memory->registerList[A] = (uint32_t)value;
}
*/
