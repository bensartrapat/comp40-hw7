/* 
 * 	Title: um_memory.h 
 * 	Name: Nga Pham, Sartrapat Saengcharoentrakul 
 * 	Username: npham02, ssaeng01
 * 	Assigment 6
 * 
 * 	Purpose: representation for universal machine memory that includes
 * 		 registers, segments, and program counter.
 * 
 */

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <uarray.h>
#include <stack.h>


#ifndef UMMEM_INCLUDED
#define UMMEM_INCLUDED

#define T umMem_T
typedef struct T *T;

struct T {
        int currID;               /* biggest segment ID that has been mapped */
        int maxID;                /* availability of segmentList */
        uint32_t** segmentList;   /* segmentList is a CArray of CArray */
        uint32_t* segment0;       /* segment0 store all the program
                                   * instructions */
        uint32_t* registerList;   /* list of register values */
        Stack_T unmapStack;       /* stack stores unmapped segment IDs to reuse
                                   * later */
};

/* initialize program from input file, return umMem_T */
extern T program_init(FILE *input);

/* free umMem_T */
extern void program_free(umMem_T *memory);

/*----------------------------------------------------------------------------*/

/* If the value at register C is not 0, then the value at register A is
   the value at register B */
extern void conditional_move(umMem_T memory, int A, int B, int C);

/* Get the value stored in segment r[b], offset r[C] and store it in
   register A */
extern void segmented_load(umMem_T memory, int A, int B, int C);

/* Put the value in register C into segment r[A], offset r[B] */
extern void segmented_store(umMem_T memory, int A, int B, int C);

/* Adds the values stored at B and C and store the result at register A */
extern void addition(umMem_T memory, int A, int B, int C);

/* Multiples the values stored at B and C and store the result at
   register A */
extern void multiplication(umMem_T memory, int A, int B, int C);

/* Divides B with C and store the result at register A */
extern void division(umMem_T memory, int A, int B, int C);

/* Bitwise operation NAND on values stored in registers B and C and store
   the value at register A */
extern void bitwise_NAND(umMem_T memory, int A, int B, int C);

/* Stops the computation */
extern void halt(umMem_T memory);

/* Create a segment with the given length from register C. The new
   segment is mapped as segment r[B] */
extern void map_segment(umMem_T memory, int B, int C);

/* Segment at r[C] is unmapped */
extern void unmap_segment(umMem_T memory, int C);

/* Stdout value from register C */
extern void output(umMem_T memory, int C);

/* Stores input taken from stdin into register C */
extern void input(umMem_T memory, int C);

/* Loads a new program to segment 0 and set the program counter to the
   value at segment 0, offset value at register C */
extern uint32_t load_program(umMem_T memory, int B, int C);

/* Stores the value (bits after 3 bits away from the opcode) at r[A] */
extern void load_value(umMem_T memory, unsigned A, unsigned value); 


#undef T 
#endif 


