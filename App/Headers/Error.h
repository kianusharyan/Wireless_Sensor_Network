/*--------------- E r r o r . h ---------------*/

#pragma once
#include "includes.h"


typedef struct errors { 
  CPU_INT08U checksum; 
  CPU_INT08U preamble1; 
  CPU_INT08U preamble2; 
  CPU_INT08U preamble3; 
  CPU_INT08U packetlength;
  CPU_INT08U destinationaddr; 
  CPU_INT08U unknown_message; 
  } errors;

void Set_Preamble1_Error();
void Set_Preamble2_Error();
void Set_Preamble2_Error();
void Set_PacketLength_Error();
void Set_Destination_Addr_Error();
void Set_Unknown_Msg_Error();

void Print_Errors();

void Synchronize();