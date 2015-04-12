/* 
* 	Title: emulator.h 
* 	Name: Nga Pham, Sartrapat Saengcharoentrakul 
* 	Username: npham02, ssaeng01
* 	Assigment 6
* 
* 	Purpose: representation of the emulator. Call execute to execute
* 		the program that is stored in segment zero.
* 
* 	Fail cases: - program counter points outside of bound of segment zero
* 		    - each word does not code for a valid instruction 
*/

#include "um_memory.h"




void execute(umMem_T memory);

void get_registers(uint32_t inst, int* rA, int* rB, int* rC);