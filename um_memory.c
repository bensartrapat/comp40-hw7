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
#define T umMem_T

struct T {
        int progCounter;
        int maxID;		/* biggest segment ID that has been mapped */
        UArray_T segmentList;	/* segmentList is a UArray of UArray */
        UArray_T registerList;
        Stack_T unmapStack;	/* stack stores unmapped segment IDs to reuse
         	 	 	 * later */
};

static inline int get_new_ID(umMem_T memory);
static inline void store_old_ID(umMem_T memory, int segID);

T program_init(FILE *input) 
{
        assert(input);
        int c, progSize;
        int shift = SHIFT_VAL;
        uint64_t word;
        
        /* initialize umMem_T */
        T memory = malloc(sizeof(struct T));
        memory->progCounter = ZERO;
        memory->maxID = ZERO;
        memory->segmentList = UArray_new(HINT, sizeof(UArray_T));
        memory->registerList = UArray_new(NUM_REGISTER, UI32SIZE);
        memory->unmapStack = Stack_new();
        
        /* initialize all registers to ZERO */
        for (int i = 0; i < NUM_REGISTER; i++) {
                register_put(memory, i, (uint32_t)ZERO);
        }
        
        /* find size of the input in bytes */
        fseek(input, 0L, SEEK_END);
        progSize = ftell(input);
        fseek(input, 0L, SEEK_SET);
        
        /* create segment zero */
        UArray_T segmentZero = UArray_new(progSize / UI32SIZE, UI32SIZE);
        *(UArray_T *)UArray_at(memory->segmentList, ZERO) = segmentZero;
        
        /* load instructions to segment zero */
        while ((c = getc(input)) != EOF) {
                if (shift > 0) {
                        shift = shift - BYTESIZE;
                        word = Bitpack_newu(word, BYTESIZE, shift, 
                        					(unsigned)c);
                }
                if (shift == 0) {
                        segment_put(memory, ZERO, progCounter_get(memory), 
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
        UArray_T curr = NULL;
		
		/* loop to free every segment in segmentList */
        for (int i = 0; i <= (*memory)->maxID; i++) {
                curr = *(UArray_T *)UArray_at((*memory)->segmentList, i);
                if (curr != NULL) {
                        UArray_free(&curr);
                }
        }
        
        UArray_free(&((*memory)->segmentList));
        UArray_free(&((*memory)->registerList));
        Stack_free(&((*memory)->unmapStack));
        free(*memory);
}

void segment_map(umMem_T memory, int regID, int length) 
{
        assert(memory);
        
        /* find an available segment ID to use */
        int newSegID = get_new_ID(memory);
        register_put(memory, regID, newSegID);

        /* resize if necessary */
        if (newSegID >= UArray_length(memory->segmentList)) {
                UArray_resize(memory->segmentList, newSegID * 2);
        } 
        
        UArray_T segment = UArray_new(length, UI32SIZE);        
        *(UArray_T *)UArray_at(memory->segmentList, newSegID) = segment;
}

void segment_unmap(umMem_T memory, int segID) 
{
        assert(memory);
        assert(segment_isEmpty(memory, segID) == false);
        
        store_old_ID(memory, segID);
        
        UArray_T segment = *(UArray_T *)UArray_at(memory->segmentList, segID);
        UArray_free(&segment);
        *(UArray_T *)UArray_at(memory->segmentList, segID) = NULL;
}

bool segment_isEmpty(umMem_T memory, int segID)
{
        assert(memory);
        
        if (*(UArray_T *)UArray_at(memory->segmentList, segID) == NULL) {
                return true;
        } else { 
                return false;
        }
}

uint32_t segment_get(umMem_T memory, int segID, int offset)
{
    	assert(segment_isEmpty(memory, segID) == false);
        assert(offset >= 0 && offset < segment_length(memory, segID));
        
        UArray_T segment = *(UArray_T *)UArray_at(memory->segmentList, segID);
        return *(uint32_t *)UArray_at(segment, offset);
}

void segment_put(umMem_T memory, int segID, int offset, uint32_t value)
{
        assert(memory);
        assert(segment_isEmpty(memory, segID) == false);
        assert(offset >= 0 && offset < segment_length(memory, segID));
		        
        UArray_T segment = *(UArray_T *)UArray_at(memory->segmentList, segID);
        *(uint32_t *)UArray_at(segment, offset) = value;
}

int segment_length(umMem_T memory, int segID)
{
        assert(memory);
        UArray_T segment = *(UArray_T *)UArray_at(memory->segmentList, segID);
        if (segment != NULL) {
                        return UArray_length(segment);
        } else { 
                return 0;
        }
}

uint32_t register_get(umMem_T memory, int regID) 
{
	assert(memory);
        uint32_t val = *(uint32_t *)UArray_at(memory->registerList, regID);
        return val;
}


void register_put(umMem_T memory, int regID, uint32_t value)
{
        assert(memory);
        *(uint32_t *)UArray_at(memory->registerList, regID) = value;
}

int progCounter_get(umMem_T memory) 
{
	assert(memory);
        return memory->progCounter;
}

void progCounter_update(umMem_T memory, int value)
{
        assert(value >= ZERO);
        assert(memory);
        memory->progCounter = value;
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