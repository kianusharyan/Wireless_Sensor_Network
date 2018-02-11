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
#include "Error.h"


void ParsePkt(void *payloadBfr)
{
	ParserState parseState = P1;
        CPU_INT08U  c;
        CPU_INT08U  i;
        CPU_INT08U  xor_sum = 0;

        
        PktBfr *pktBfr = payloadBfr;

        for(;;)
        {
          
            c = GetByte();

          switch(parseState)
          {
          
          case P1: //Preamble 1 0x03
             if (c == P1Char)
             {
                parseState = P2;
                xor_sum =  c;
             }
             else
             {
                Print_Preamble1_Error();
                Synchronize();
                xor_sum = P1Char ^ P2Char ^P3Char;
                //BSP_Ser_Printf("P1 recovered xor_sum: %x \n", xor_sum);

                parseState = K;
             }
             break;
          case P2: //Preamble 2 0xEF
             if (c == P2Char)
             {
               parseState = P3;
               xor_sum = xor_sum ^ c;
             }
             else
             {
                Print_Preamble2_Error();
                Synchronize();
                xor_sum = P1Char ^ P2Char ^P3Char;
                parseState = K;

             }
             break;
          case P3: //Preamble 2 0xAF
             if (c == P3Char)
             {
               xor_sum = xor_sum ^ c;
               parseState = K; //next byte is length

             }
             else
             {
                Print_Preamble3_Error();
                Synchronize();
                xor_sum = P1Char ^ P2Char ^P3Char;
                parseState = K;

             }
             break;
          case K:
            if (c < 8)
            {
                Print_PacketLength_Error();
                Synchronize();
                xor_sum = P1Char ^ P2Char ^P3Char; //accumulate preambles
                parseState = K; //first 3 preambles have been read
            }
            else //length is ok
            {    
            pktBfr->payLoadLen = c - HeaderLength;
            xor_sum = xor_sum ^ c;
            parseState = D;
            } 
            i = 0; //start at first index every time this state is completed
            break;
          case D:
            
            pktBfr->data[i] = c;
            if (pktBfr->data[0] != 0x01)
            {
                Print_Destination_Addr_Error();
                Synchronize();
                xor_sum = P1Char ^ P2Char ^P3Char;
                parseState = K;
            }
            if (i < pktBfr->payLoadLen - 1)
            {
              xor_sum = xor_sum ^ c;        //collecting checksum data
            }

            if (i >= pktBfr->payLoadLen - 1)
            {

              if(c != xor_sum)
              {
                Print_Checksum_Error();
                Synchronize();
                xor_sum = P1Char ^ P2Char ^P3Char;
                parseState = K;

              } 
              else{
                return;
              }
            }
            i++;
            break;
          default:
            parseState = K;

          }//end switch
          
        }//end forever loop
        
}//end routine