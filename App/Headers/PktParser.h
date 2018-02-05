/*--------------- P k t P a r s e r . h ---------------*/

#ifndef PktParser_H
#define PktParser_H

//#pragma once

/*----- c o n s t a n t    d e f i n i t i o n s -----*/

#define HeaderLength 4
#define P1Char 0x03
#define P2Char 0xEF
#define P3Char 0xAF


//Parser state data type
typedef enum { P1, P2, P3, T, K, D, ER } ParserState;

//Packer structure data type 
  

/*----- f u n c t i o n    p r o t o t y p e s -----*/

CPU_INT08U GetByte();

void ParsePkt(void *payloadBfr);

typedef struct
{
	CPU_INT08U payLoadLen;
	CPU_INT08U data[1];
} PktBfr;

#endif