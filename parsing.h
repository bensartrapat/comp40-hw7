/* 
 * 	Title: parsing.h 
 * 	Name: Nga Pham, Sartrapat Saengcharoentrakul 
 * 	Username: npham02, ssaeng01
 * 	Assigment 6
 * 
 * 	Purpose: parse register values and opcode from uint32 word
 */

#include <inttypes.h>

/* return opcode value from the word */
int get_opcode(uint32_t word);

/* If opcode is from 0 to 12, use these functions to get indices
 * of register A, B, C 
 */
int get_rA(uint32_t word);

int get_rB(uint32_t word);

int get_rC(uint32_t word);

/* If opcode is 13, use these functions to get register A, and 
 * the least significant 25bit value */
int opcode_13_get_rA(uint32_t word);

int opcode_13_get_value(uint32_t word);
