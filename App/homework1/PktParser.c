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
            //BSP_Ser_Printf("GetByte c: %x \n", c);

            xor_sum = xor_sum ^ c;
          switch(parseState)
          {
          
          case P1: //Preamble 1 0x03
             if (c == P1Char)
             {
                parseState = P2;
                xor_sum = xor_sum ^ c;
             }
             else
             {
                Set_Preamble1_Error();
                Print_Errors();
                Synchronize();
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
                Set_Preamble2_Error();
                Print_Errors();
                Synchronize();
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
                Set_Preamble3_Error();
                Print_Errors();
                Synchronize();
                parseState = K;

             }
             break;
          case K:
            pktBfr->payLoadLen = c - HeaderLength;
            if (pktBfr->payLoadLen > 8)
            {
                Set_PacketLength_Error;
                Print_Errors();
                Synchronize();
                parseState = K;

            }
            xor_sum = xor_sum ^ c;
            parseState = D;
            i = 0;
            break;
          case D:
            pktBfr->data[i++] = c;

            //BSP_Ser_Printf("c: %x \n", c);
            //BSP_Ser_Printf("pktBfr->data:  %x \n", pktBfr->data[i]);
            //BSP_Ser_Printf("xor_sum: %x \n", xor_sum);

            if(pktBfr->data[0] != 0x01)
            {
                Set_Destination_Addr_Error();
                Print_Errors();
                Synchronize();
                parseState = K;

            }
            if (i < pktBfr->payLoadLen)
            {
              xor_sum = xor_sum ^ c;
              //BSP_Ser_Printf("if xor_sum: %x \n", xor_sum);
            }

            if (i >= pktBfr->payLoadLen)
            {

              if(c != xor_sum)
              {
                Set_Checksum_Error();
                Print_Errors();
                Synchronize();
                parseState = K;

              } 
              else{
                return;
              }
            }
            break;
          default:
            parseState = K;

          }//end switch
          
        }//end forever loop
        
}//end routine
        
        
        void Synchronize()
{

    CPU_INT08U temp = 0;
    CPU_INT08U preamble1confirmed = 0;
    CPU_INT32S checksum = 0;
        
    while(1)
    {
             temp = GetByte();
             //BSP_Ser_Printf(" temp initial: %x \n", temp);

             if ( temp == P1Char) //skip until first three legit preamble byte
             {
                 temp = GetByte();
                // BSP_Ser_Printf("preamble 2: %x \n", temp);

                 if ( temp == P2Char) //skip until first three legit preamble byte
                 {
                    temp = GetByte();
                   // BSP_Ser_Printf(" preamble 3: %x \n", temp);
                     if ( temp == P3Char) //skip until first three legit preamble byte
                     {
                     // BSP_Ser_Printf(" recovered: %x \n", temp);
                      return;
                     }
                 }
             }
   }

}