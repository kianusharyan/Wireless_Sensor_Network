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

static errors err;              //linker doesn't have to know about this, it keeps its values between calls

void Set_Checksum_Error()
{
  err.checksum = 1;
}

void Set_Unknown_Msg_Error()
{
  err.unknown_message = 1;
}

void Set_Preamble1_Error()
{
    err.preamble1 = 1;
}

void Set_Preamble2_Error()
{
    err.preamble2 = 1;
}

void Set_Preamble3_Error()
{
    err.preamble3 = 1;
}

void Set_PacketLength_Error()
{
  err.packetlength= 1;
}

void Set_Destination_Addr_Error()
{

  err.destinationaddr = 1;
}

void Clear_Errors()
{
   err.checksum = 0;
  err.unknown_message = 0;
  err.preamble1 = 0;
  err.preamble2 = 0;
  err.preamble3 = 0;
  err.destinationaddr = 0;
}

void Print_Errors()
{
  if (err.preamble1 == 1)
     BSP_Ser_Printf("\a*** ERROR: Bad Preamble Byte 1\n");
  
  if (err.preamble2 == 1)
     BSP_Ser_Printf("\a*** ERROR: Bad Preamble Byte 2\n");
  
  if (err.preamble3 == 1)
     BSP_Ser_Printf("\a*** ERROR: Bad Preamble Byte 3\n");
  
  if (err.checksum == 1)
     BSP_Ser_Printf("\a*** ERROR: Checksum Error\n");
  
  if (err.destinationaddr == 1)
     BSP_Ser_Printf("\a*** INFO: Not My Address\n");

  if (err.packetlength == 1)
     BSP_Ser_Printf("\a*** ERROR:  Bad Packet Size\n");

  if (err.unknown_message == 1)
     BSP_Ser_Printf("\a*** ERROR: Unknown Message Type\n");

Clear_Errors();
}



