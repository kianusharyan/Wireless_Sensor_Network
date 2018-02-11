/*--------------- E r r o r . c ---------------*/

/*
by:	Kianush Aryan
	  Embedded Real Time Systems
	  Electrical and Computer Engineering Dept.
	  UMASS Lowell

PURPOSE
Sets error flags and Holds sync and print function implementation.

*/
#include "Error.h"
#include "PktParser.h"
            
void Synchronize()
{

    CPU_INT08U temp = 0;
        
    while(1)
    {
             temp = GetByte();

             if ( temp == P1Char) //skip until first three legit preamble byte
             {
                 temp = GetByte();

                 if ( temp == P2Char) //skip until first three legit preamble byte
                 {
                    temp = GetByte();
                     if ( temp == P3Char) //skip until first three legit preamble byte
                     {
                      return;
                     }
                 }
             }
   }

}

void Print_Checksum_Error()
{
    BSP_Ser_Printf("\a*** ERROR: Checksum Error\n\n");
}

void Print_Unknown_Msg_Error()
{
     BSP_Ser_Printf("\a*** ERROR: Unknown Message Type\n\n");
}

void Print_Preamble1_Error()
{
     BSP_Ser_Printf("\a*** ERROR: Bad Preamble Byte 1\n\n");
}

void Print_Preamble2_Error()
{
     BSP_Ser_Printf("\a*** ERROR: Bad Preamble Byte 2\n\n");
}

void Print_Preamble3_Error()
{
     BSP_Ser_Printf("\a*** ERROR: Bad Preamble Byte 3\n\n");
}

void Print_PacketLength_Error()
{
     BSP_Ser_Printf("\a*** ERROR:  Bad Packet Size\n\n");
}

void Print_Destination_Addr_Error()
{
     BSP_Ser_Printf("\a*** INFO: Not My Address\n\n");
}