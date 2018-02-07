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

static errors err;

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

void Print_Errors()
{
  if (err.preamble1 == 1)
     BSP_Ser_Printf("\a*** ERROR: Bad Preamble Byte 1");
  
  if (err.preamble2 == 1)
     BSP_Ser_Printf("\a*** ERROR: Bad Preamble Byte 2");
  
  if (err.preamble3 == 1)
     BSP_Ser_Printf("\a*** ERROR: Bad Preamble Byte 3");
  
  if (err.checksum == 1)
     BSP_Ser_Printf("\a*** ERROR: Checksum Error");
  
  if (err.destinationaddr == 1)
     BSP_Ser_Printf("\a*** INFO: Not My Address");

  if (err.packetlength == 1)
     BSP_Ser_Printf("\a*** ERROR:  Bad Packet Size");

  if (err.unknown_message == 1)
     BSP_Ser_Printf("\a*** ERROR: Unknown Message Type");


}
