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
//         uint32_t* data = calloc(progSize / UI32SIZE + 1, UI32SIZE);
// //         data[0] = progSize;
//         *(uint32_t **)UArray_at(memory->segmentList, ZERO) = data;

        memory->segment0 = calloc(progSize/UI32SIZE, UI32SIZE);

//         fprintf(stderr, "progSize is: %i\n", progSize/UI32SIZE);
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
//                         if (counter == 4777) fprintf(stderr, "HEY!!!!!!!!");
//  if (counter == 4654) fprintf(stderr, "word is %"PRIu32"\n", word);
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
        for (int i = 0; i <= (*memory)->maxID; i++) {
                curr = *(uint32_t **)UArray_at((*memory)->segmentList, i);
                if (curr != NULL) {
                        free(curr);
                }
        }
        
        UArray_free(&((*memory)->segmentList));
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
        uint32_t* segment = *(uint32_t**)UArray_at(memory->segmentList,
                                        memory->registerList[B]);
        memory->registerList[A] = segment[memory->registerList[C]+1];
        }
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
//          fprintf(stderr, "in SEGMENT_MAP\n");
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
//                         fprintf(stderr, "IN LOAD PROGRAM!!\n");
//                 segment_unmap(memory, 0);
//                 uint32_t* segment =
// *(uint32_t**)UArray_at(memory->segmentList,
//                                         rB_value);
//                 int length = segment[0];
//                 segment_map(memory, 0, length);
                
                /* copy from segment with ID $r[B] to segment 0 */
//                 for (int i = 0; i < length; i++) {
//                         uint32_t value = segment[i+1];//segment_get(memory,
// rB_value, i);
//                         segment_put(memory, 0, i, value);
                free(memory->segment0);
                uint32_t* segment =
 *(uint32_t**)UArray_at(memory->segmentList,
                                         rB_value);
                int length = segment[0];
//                 fprintf(stderr, "length %i\n", length);
                memory->segment0 = calloc(length, UI32SIZE);
                
                for (int i = 0; i<length; i++) {
                        memory->segment0[i] = segment[i+1];
                }
        }
//         fprintf(stderr, "new counter is: %u\n", memory->registerList[C]);
        return memory->registerList[C];
}

void load_value(umMem_T memory, unsigned A, unsigned value)
{
        memory->registerList[A] = (uint32_t)value;
}
/*----------------------------------------------------------------------------*/
void segment_map(umMem_T memory, int regID, int length) 
{
        assert(memory);
//         fprintf(stderr, "in SEGMENT_MAP\n");
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
        if (segID == 0) {
//                 fprintf(stderr, "SEGMENT_GET segID: %i, offset:"
// "%i\n",segID,offset);
                return memory->segment0[offset];
        } else {
                uint32_t* segment = *(uint32_t **)UArray_at(memory->segmentList,
segID);
        assert(segment != NULL);
        assert(offset >= 0 && offset < (int)segment[0]);
        return segment[offset+1];
        }
}

void segment_put(umMem_T memory, int segID, int offset, uint32_t value)
{
        assert(memory);
//         fprintf(stderr, "segID: %i, offset: %i\n",segID,offset);
        if (segID == 0) {
                memory->segment0[offset] = value;
        } else {
                uint32_t* segment = *(uint32_t **)UArray_at(memory->segmentList,
segID);
        assert(segment != NULL);
        assert(offset >= 0 && offset < (int)segment[0]);
        
        segment[offset+1] = value;
        }

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
