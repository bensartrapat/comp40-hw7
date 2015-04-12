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

const int NUM_REGISTER = 8; 
const int BYTESIZE = 8;
const int HINT = 32;
const int BYTE_UINT32 = 4; /* number of bytes in 32 bits */
const int SHIFT_VAL = 32; 
const int ZERO = 0;
const int UI32SIZE = sizeof(uint32_t);

static inline int get_new_ID(umMem_T memory);
static inline void store_old_ID(umMem_T memory, int segID);

umMem_T program_init(FILE *input) 
{
        assert(input);
        int c, progSize = 0;
        int shift = SHIFT_VAL;
        uint64_t word = 0;
        
        /* initialize umMem_T */
        umMem_T memory = malloc(sizeof(struct umMem_T));
        memory->progCounter = ZERO;
        memory->maxID = ZERO;
        memory->segmentList = UArray_new(HINT, sizeof(UArray_T));
        memory->registerList = calloc(NUM_REGISTER, sizeof(uint32_t));
        memory->unmapStack = Stack_new();
        
        /* initialize all registers to ZERO */
        for (int i = 0; i < NUM_REGISTER; i++) {
                memory->registerList[i] = ZERO;
        }
        
        /* find size of the input in bytes */
        fseek(input, 0L, SEEK_END);
        progSize = ftell(input);
        fseek(input, 0L, SEEK_SET);
        
        /* create segment zero */
        uint32_t* data = calloc(progSize / UI32SIZE + 1, UI32SIZE);
        data[0] = progSize;
        *(uint32_t **)UArray_at(memory->segmentList, ZERO) = data;
        
        /* load instructions to segment zero */
        while ((c = getc(input)) != EOF) {
                if (shift > 0) {
                        shift = shift - BYTESIZE;
                        word = Bitpack_newu(word, BYTESIZE, shift, 
                        					(unsigned)c);
                }
                if (shift == 0) {
                        segment_put(memory, ZERO, memory->progCounter, 
                                                  (uint32_t)word);
                        shift = SHIFT_VAL;
                        memory->progCounter++;
                }
        }                           

        memory->progCounter = ZERO;
        
        return memory;
}

void program_free(umMem_T *memory) 
{
        assert(memory);
        uint32_t* curr = NULL;
		
		/* loop to free every segment in segmentList */
        for (int i = 0; i <= (*memory)->maxID; i++) {
                curr = *(uint32_t **)UArray_at((*memory)->segmentList, i);
                if (curr != NULL) {
                        free(curr);
                }
        }
        
        UArray_free(&((*memory)->segmentList));
        free((*memory)->registerList);
        Stack_free(&((*memory)->unmapStack));
        free(*memory);
}

void segment_map(umMem_T memory, int regID, int length) 
{
        assert(memory);
        
        /* find an available segment ID to use */
        int newSegID = get_new_ID(memory);
        memory->registerList[regID] = newSegID;

        /* resize if necessary */
        if (newSegID >= UArray_length(memory->segmentList)) {
                UArray_resize(memory->segmentList, newSegID * 2);
        } 
        
        uint32_t* data = calloc(length+1, UI32SIZE);
        data[0] = length;
        *(uint32_t **)UArray_at(memory->segmentList, newSegID) = data;
}

void segment_unmap(umMem_T memory, int segID) 
{
        assert(memory);
        //assert(segment_isEmpty(memory, segID) == false);
        
        store_old_ID(memory, segID);
        
        uint32_t* segment = *(uint32_t **)UArray_at(memory->segmentList, segID);
        assert(segment != NULL);
        free(segment);
        *(uint32_t **)UArray_at(memory->segmentList, segID) = NULL;
}

bool segment_isEmpty(umMem_T memory, int segID)
{
        assert(memory);
        
        if (*(uint32_t **)UArray_at(memory->segmentList, segID) == NULL)
                return true;
        return false;
}

uint32_t segment_get(umMem_T memory, int segID, int offset)
{
    	//assert(segment_isEmpty(memory, segID) == false);
        uint32_t* segment = *(uint32_t **)UArray_at(memory->segmentList, segID);
        assert(segment != NULL);
        assert(offset >= 0 && offset < (int)segment[0]);
        
        
        return segment[offset+1];
}

void segment_put(umMem_T memory, int segID, int offset, uint32_t value)
{
        assert(memory);
        //assert(segment_isEmpty(memory, segID) == false);
        
        uint32_t* segment = *(uint32_t **)UArray_at(memory->segmentList, segID);
        assert(segment != NULL);
        assert(offset >= 0 && offset < (int)segment[0]);
        
        segment[offset+1] = value;

}

int segment_length(umMem_T memory, int segID)
{
        assert(memory);
        uint32_t* segment = *(uint32_t**)UArray_at(memory->segmentList,
segID);
        if (segment != NULL) {
                return segment[0];
        } else { 
                return 0;
        }
}

uint32_t register_get(umMem_T memory, int regID) 
{
	assert(memory);
//         uint32_t val = *(uint32_t *)UArray_at(memory->registerList, regID);
        return memory->registerList[regID];
}


void register_put(umMem_T memory, int regID, uint32_t value)
{
        assert(memory);
//         *(uint32_t *)UArray_at(memory->registerList, regID) = value;
        memory->registerList[regID] = value;
}

/* return an ID that can be used to map a segment
 * if unmapStack has available IDs to reuse, return an ID from that
 * else, return an ID to map segment at the end of the segmentList
 */ 
static int get_new_ID(umMem_T memory)
{
        int newID;
        if (Stack_empty(memory->unmapStack) == 1) { /* 1 means empty */
                newID = memory->maxID + 1;
                memory->maxID++;
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
