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


#ifndef UMMEM_INCLUDED
#define UMMEM_INCLUDED

#define T umMem_T
typedef struct T *T;

/* initialize program from input file, return umMem_T */
extern T program_init(FILE *input);

/* free umMem_T */
extern void program_free(umMem_T *memory);

/* map segment with given length, store mapped segment ID in 
 * given register regID 
 */
extern void segment_map(umMem_T memory, int regID, int length);

/* unmap segment with segID */
extern void segment_unmap(umMem_T memory, int segID);

/* check if segment has been mapped yet */
extern bool segment_isEmpty(umMem_T memory, int segID);

/* return a segment length */
extern int segment_length(umMem_T memory, int ID);

/* get or put an instruction in segment segID and at offset */
extern uint32_t segment_get(umMem_T memory, int segID, int offset);

extern void segment_put(umMem_T memory, int segID, int offset, uint32_t value);

/* get or put value in register regID */
extern uint32_t register_get(umMem_T memory, int regID);

extern void register_put(umMem_T memory, int regID, uint32_t value);

/* get or update program counter, what instruction you are in segment 0 */
extern int progCounter_get(umMem_T memory);

extern void progCounter_update(umMem_T memory, int value);

#undef T 
#endif 


