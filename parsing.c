/* 
 * 	Title: parsing.c 
 * 	Name: Nga Pham, Sartrapat Saengcharoentrakul 
 * 	Username: npham02, ssaeng01
 * 	Assigment 6
 * 
 * 	Purpose: implementation of parsing.h, using types and functions 
 * 		 from bitpack.h
 */


#include "parsing.h"
#include "bitpack.h"
#include <inttypes.h>

static const int OPCODE_WIDTH = 4;
static const int REGISTER_WIDTH = 3;

int get_opcode(uint32_t word) 
{
        return Bitpack_getu(word, OPCODE_WIDTH, 28);
}

int get_rA(uint32_t word)
{
        return Bitpack_getu(word, REGISTER_WIDTH, 6);
}

int get_rB(uint32_t word)
{
        return Bitpack_getu(word, REGISTER_WIDTH, 3);
}

int get_rC(uint32_t word)
{
        return Bitpack_getu(word, REGISTER_WIDTH, 0);
}

int opcode_13_get_rA(uint32_t word)
{
        return Bitpack_getu(word, REGISTER_WIDTH, 25);        
}

int opcode_13_get_value(uint32_t word)
{
        return Bitpack_getu(word, 25, 0);       
}
