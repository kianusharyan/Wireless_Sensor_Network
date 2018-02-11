/*--------------- P k t P a r s e r . h ---------------*/


//#don't redefine this header
#pragma pack(1)

/*----- c o n s t a n t    d e f i n i t i o n s -----*/

#define HeaderLength 4
#define P1Char 0x03
#define P2Char 0xEF
#define P3Char 0xAF


//Parser state data type
typedef enum { P1, P2, P3, T, K, D, SYNC} ParserState;

//Packer structure data type 
  typedef struct
{
	CPU_INT08U payLoadLen;
	CPU_INT08U data[1];
} PktBfr;

/*----- f u n c t i o n    p r o t o t y p e s -----*/
void ParsePkt(void *payloadBfr);
