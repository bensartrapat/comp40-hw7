/* 
 * 	Title: um_memory.c 
 * 	Name: Nga Pham, Sartrapat Saengcharoentrakul 
 * 	Username: npham02, ssaeng01
 * 	Assigment 6
 * 
 * 	Purpose: implementation for universal machine memory (i.e. registers,
 * 		 segments) using uarrays, stack, and bitpack types and
 * 		 functions
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#include "um_memory.h"
#include "uarray.h"
#include "assert.h"
#include "bitpack.h"
#include "stack.h"
#include <math.h>


const int NUM_REGISTER = 8; 
const int BYTESIZE = 8;
const int HINT = 32;
const int BYTE_UINT32 = 4; /* number of bytes in 32 bits */
const int SHIFT_VAL = 32; 
const int ZERO = 0;
const int UI32SIZE = sizeof(uint32_t);
const uint32_t NOT_ONE = ~0;

static inline int get_new_ID(umMem_T memory);
static inline void store_old_ID(umMem_T memory, int segID);

umMem_T program_init(FILE *input) 
{
        assert(input);
        int c, progSize = 0;
        int shift = SHIFT_VAL;
        uint32_t word = 0;
        
        /* initialize umMem_T */
        umMem_T memory = malloc(sizeof(struct umMem_T));
        memory->currID = ZERO;
        memory->maxID = HINT;
        memory->segmentList = calloc(memory->maxID, sizeof(uint32_t*));
        memory->registerList = calloc(NUM_REGISTER, sizeof(uint32_t));
        memory->unmapStack = Stack_new();
        
        /* initialize all registers to ZERO */
        for (int i = 0; i < NUM_REGISTER; i++) {
                memory->registerList[i] = ZERO;
        }
        
        /*initilize all segments to NULL */
        for (int i = 0; i < memory->maxID; i++) {
                memory->segmentList[i] = NULL;
        }
        
        /* find size of the input in bytes */
        fseek(input, 0L, SEEK_END);
        progSize = ftell(input);
        fseek(input, 0L, SEEK_SET);
        
        /* create segment zero */
        memory->segment0 = calloc(progSize/UI32SIZE, UI32SIZE);

        /* load instructions to segment zero */
        uint32_t counter = ZERO;
        while ((c = getc(input)) != EOF) {
                if (shift > 0) {
                        shift = shift - BYTESIZE;
                        word = Bitpack_newu(word, BYTESIZE, shift, 
                        					(unsigned)c);
                }
                if (shift == 0) {
                        memory->segment0[counter] = word;
                        shift = SHIFT_VAL;
                        counter++;
                }
        }                        
        return memory;
}

void program_free(umMem_T *memory) 
{
        assert(memory);
        uint32_t* curr = NULL;
        
        /* loop to free every segment in segmentList */
        for (int i = 0; i <= (*memory)->currID; i++) {
                curr = (*memory)->segmentList[i];
                if (curr != NULL) {
                        free(curr);
                }
        }
        
        free((*memory)->segmentList);
        free((*memory)->segment0);
        free((*memory)->registerList);
        Stack_free(&((*memory)->unmapStack));
        free(*memory);
}

/*----------------------------------------------------------------------------*/
void conditional_move(umMem_T memory, int A, int B, int C)
{
        if (memory->registerList[C] != 0) {
                memory->registerList[A] = memory->registerList[B];
        }               
}

void segmented_load(umMem_T memory, int A, int B, int C)
{
        if (memory->registerList[B] == 0) {
                memory->registerList[A] =
                memory->segment0[memory->registerList[C]];
        } else {
                memory->registerList[A] =
                memory->segmentList[memory->registerList[B]]        
                                   [memory->registerList[C]+1];     
        }
}

void segmented_store(umMem_T memory, int A, int B, int C)
{
        if (memory->registerList[A] == 0) {
                memory->segment0[memory->registerList[B]] =
                                                memory->registerList[C];
        } else {
                memory->segmentList[memory->registerList[A]]
                                   [memory->registerList[B]+1]
                = memory->registerList[C];
        }

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
        int regID = B;
        int length = memory->registerList[C];
        
        int newSegID = get_new_ID(memory);
        memory->registerList[regID] = newSegID;

        /* resize if necessary */
        int new_maxID = memory->maxID * 2;
        if (newSegID >= memory->maxID) {
                uint32_t** tempSegList = calloc(new_maxID,
                sizeof(uint32_t*));                
                for (int i = 0; i < memory->maxID; i++) {
                        uint32_t* oldCurr = memory->segmentList[i];
                        tempSegList[i] = oldCurr;
                       
                        if (oldCurr != NULL) { 
                                oldCurr = NULL;
                        }
                }
                /*initilize all segments to NULL */
                for (int i = memory->maxID; i < new_maxID; i++) {
                        tempSegList[i] = NULL;
                }
                free(memory->segmentList);
                memory->maxID = new_maxID;
                memory->segmentList = tempSegList;
        } 
        
        uint32_t* data = calloc(length+1, UI32SIZE);
        data[0] = length;
        memory->segmentList[newSegID] = data;
}

void unmap_segment(umMem_T memory, int C)
{
        assert(memory->registerList[C] != 0);
        int segID = memory->registerList[C];
        store_old_ID(memory, segID);
        uint32_t* segment = memory->segmentList[segID];
        assert(segment != NULL);
        free(segment);
        memory->segmentList[segID] = NULL;
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
                free(memory->segment0);
                int length = memory->segmentList[rB_value][0];
                memory->segment0 = calloc(length, UI32SIZE);
                for (int i = 0; i<length; i++) {
                        memory->segment0[i] =
                                        memory->segmentList[rB_value][i+1];
                }
        }
        return memory->registerList[C];
}

void load_value(umMem_T memory, unsigned A, unsigned value)
{
        memory->registerList[A] = (uint32_t)value;
}

/*----------------------------------------------------------------------------*/
/* return an ID that can be used to map a segment
 * if unmapStack has available IDs to reuse, return an ID from that
 * else, return an ID to map segment at the end of the segmentList
 */ 
static int get_new_ID(umMem_T memory)
{
        int newID = 0;
        if (Stack_empty(memory->unmapStack) == 1) { /* 1 means empty */
                newID = memory->currID + 1;
                memory->currID++;
        } else {
                newID = (uintptr_t)Stack_pop(memory->unmapStack);
        }
        return newID;
}

/* store ID of an unmapped segment in unmapStack to reuse later */
static void store_old_ID(umMem_T memory, int segID)
{
        Stack_push(memory->unmapStack, (void *)(uintptr_t)segID);
}
