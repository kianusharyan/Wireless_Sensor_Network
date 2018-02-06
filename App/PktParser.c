/*--------------- P k t P a r s e r . c ---------------*/

/*
by:	Kianush Aryan
	  Embedded Real Time Systems
	  Electrical and Computer Engineering Dept.
	  UMASS Lowell

PURPOSE
Reads a void pointer to payload packet from the Rx, 
adjusts to proper type, depending on message parameters.

*/
#include "includes.h"

#include "PktParser.h"


void ParsePkt(void *payloadBfr)
{
	ParserState parseState = P1;
        CPU_INT08U  c;
        CPU_INT08U  i;
        CPU_INT08U  xor_sum = 0;
        
        PktBfr *pktBfr = payloadBfr;
//        BSP_Ser_Printf("\nHEX:\n");
//        BSP_Ser_Printf("\n%d\n", P1Char);
//        BSP_Ser_Printf("\n%x\n", P1Char);
//            
//        
//        BSP_Ser_Printf("\nHEX:\n");
//        BSP_Ser_Printf("\n%d\n", P2Char);
//        BSP_Ser_Printf("\n%x\n", P2Char);
//        
//        BSP_Ser_Printf("\nHEX:\n");
//        BSP_Ser_Printf("\n%d\n", P3Char);
//        BSP_Ser_Printf("\n%x\n", P3Char);
        for(;;)
        {
            c = GetByte();
//            BSP_Ser_Printf("\nRead:\n");
//            BSP_Ser_Printf("\n%x\n", c);
//            BSP_Ser_Printf("\n%d\n", c);
            xor_sum = xor_sum ^ c;
          switch(parseState)
          {
          
          case P1: //Preamble 1 0x03
             if (c == P1Char)
             {
                parseState = P2;
                xor_sum = xor_sum ^ c;
                           // BSP_Ser_Printf("xor_sum: %x \n", xor_sum);

                
             }
             else
             {
                parseState = ER;
                BSP_Ser_Printf("\nERR 1 DETECTED\n");
                //todo: add BYTE 1Err handling
             }
             break;
          case P2: //Preamble 2 0xEF
             if (c == P2Char)
             {
               parseState = P3;
               xor_sum = xor_sum ^ c;
                             //BSP_Ser_Printf("xor_sum: %x \n", xor_sum);

             }
             else
             {
               parseState = ER;
                BSP_Ser_Printf("\nERR 2 DETECTED\n");
                //todo: add BYTE 2 Err handling
             }
             break;
          case P3: //Preamble 2 0xAF
             if (c == P3Char)
             {
               xor_sum = xor_sum ^ c;
               parseState = K; //next byte is length
                             //BSP_Ser_Printf("xor_sum: %x \n", xor_sum);

             }
             else
             {
               parseState = ER;
                BSP_Ser_Printf("\nERR 3 DETECTED\n");
                //todo: add BYTE 2 Err handling
             }
             break;
          case K:
            pktBfr->payLoadLen = c - HeaderLength;
            xor_sum = xor_sum ^ c;
            //BSP_Ser_Printf("payLoadLen:\n%x\n", c);                      9
            //BSP_Ser_Printf("payLoadLen:\n%x\n", pktBfr->payLoadLen);        5

            parseState = D;
            i = 0;
            break;
          case D:
            pktBfr->data[i++] = c;

            BSP_Ser_Printf("c: %x \n", c);
            BSP_Ser_Printf("pktBfr->data:  %x \n", pktBfr->data[i]);
            //BSP_Ser_Printf("xor_sum: %x \n", xor_sum);


            if (i < pktBfr->payLoadLen)
            {
              xor_sum = xor_sum ^ c;
              //BSP_Ser_Printf("if xor_sum: %x \n", xor_sum);
            }

            if (i >= pktBfr->payLoadLen)
            {
             // parseState = P1;
              return;
            }
            break;
          case ER:
            if (c == P1Char)
              parseState = P2;
            break;
          }//end switch
          
        }//end forever loop
        
}//end routine