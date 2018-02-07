/*--------------- P r o g 1 . c ---------------*/

/*
by:	Kianush Aryan
	  Embedded Real Time Systems
	  Electrical and Computer Engineering Dept.
	  UMASS Lowell

PURPOSE
Contains main program and any functions init that must be called for 
for program execution.

*/

/* Include Micrium and STM headers. */
#include "includes.h"
#include "Error.h"

/*----- c o n s t a n t    d e f i n i t i o n s -----*/

#define BaudRate 9600           /* RS232 Port Baud Rate */
#pragma pack(1) //Don't align on word boundaries


/*----- Structures ------*/
typedef struct
{
    CPU_INT08U  payloadLen;
    CPU_INT08U  dstAddr;
    CPU_INT08U  srcAddr;
    CPU_INT08U  msgType;
    
    union
    {
      CPU_INT08S  temp;
      CPU_INT16U  pres;
      struct
      {
        CPU_INT08S  dewPt;
        CPU_INT08U  hum;
      } hum;
      struct
      {
        CPU_INT08U  speed[2]; //don't use hard coded
        CPU_INT16U  dir;
      } wind;
      CPU_INT16U    rad;
      CPU_INT32U    dateTime;
      CPU_INT08U    depth[2]; //don't use hard coded
      CPU_INT08U    id[10];    //don't use hard coded
    } dataPart;
} Payload;

/*----- f u n c t i o n    p r o t o t y p e s -----*/

int AppMain();

CPU_INT16U convertEndianess(CPU_INT16U convert);
CPU_INT08U convertBCD(CPU_INT08U noncoded);
void PrintTemprature(Payload payload);
void PrintPressure(Payload payload);
void PrintHumidity(Payload payload);
void PrintWind(Payload payload);
void PrintRadiation(Payload payload);
void PrintDateTime(Payload payload);
void PrintPrecipitation(Payload payload);
void PrintID(Payload payload);


/*--------------- m a i n ( ) -----------------*/

CPU_INT32S main()
{
    CPU_INT32S	exitCode;       // Return this code on exit.
	
//  Initialize the STM32F107 eval. board.
    BSP_IntDisAll();            /* Disable all interrupts. */

    BSP_Init();                 /* Initialize BSP functions */

    BSP_Ser_Init(BaudRate);     /* Initialize the RS232 interface. */

//  Run the application.    
    exitCode = AppMain();

    
    return exitCode;
}

/*--------------- A p p M a i n ( ) -----------------*/

CPU_INT32S AppMain()
{

 BSP_Ser_Printf("\n*************************************PROGRAM STARTED...*************************************\n");

  Payload payload;
  

  while(1)
  {
      ParsePkt(&payload);

      switch(payload.msgType)
      {
      case 1: //temprature
         PrintTemprature(payload);
         break;
      case 2: //pressure
         PrintPressure(payload);
         break;
      case 3: //humidity
         PrintHumidity(payload);
         break;
      case 4: //wind
         PrintWind(payload);
         break;
      case 5: //radiation
        PrintRadiation(payload);
        break;
      case 6: //date and time
        PrintDateTime(payload);
        break;
      case 7: //precipitation
        PrintPrecipitation(payload);
        break;
      case 8: //ID message
        PrintID(payload);
        break;
      default:
        Set_Unknown_Msg_Error;
        Print_Errors();
        break; //redundancy
      }
      
      

  } 
  

  BSP_Ser_Printf("\ns*************************************PROGRAM ENDED...*************************************n");

  return 0;
}

CPU_INT08U convertBCD(CPU_INT08U noncoded)
{
  return noncoded;
}
CPU_INT16U convertEndianess(CPU_INT16U convert)
{
  return convert;
}
void PrintTemprature(Payload payload){
          BSP_Ser_Printf("payload.payloadLen \n %x \n", payload.payloadLen);
          BSP_Ser_Printf("payload.dstAddr \n %x \n", payload.dstAddr);
          BSP_Ser_Printf("payload.srcAddr \n %x \n", payload.srcAddr);
          BSP_Ser_Printf("payload.msgType \n %x \n", payload.msgType);
          BSP_Ser_Printf("SOURCE NODE 2: TEMPERATURE MESSAGE \n Temperature = %d \n", payload.dataPart.temp);
}
void PrintPressure(Payload payload)
{
          CPU_INT16U reversed_pressure = ((payload.dataPart.pres & 0x00FF) << 8) | ((payload.dataPart.pres & 0xFF00) >> 8);
          BSP_Ser_Printf("payload.payloadLen \n %x \n", payload.payloadLen);
          BSP_Ser_Printf("payload.dstAddr \n %x \n", payload.dstAddr);
          BSP_Ser_Printf("payload.srcAddr \n %x \n", payload.srcAddr);
          BSP_Ser_Printf("payload.msgType \n %x \n", payload.msgType);
          BSP_Ser_Printf("SOURCE NODE 3: BAROMETRIC PRESSURE MESSAGE \n Pressure = %d \n", reversed_pressure);

}
void PrintHumidity(Payload payload)
{
          BSP_Ser_Printf("payload.payloadLen \n %x \n", payload.payloadLen);
          BSP_Ser_Printf("payload.dstAddr \n %x \n", payload.dstAddr);
          BSP_Ser_Printf("payload.srcAddr \n %x \n", payload.srcAddr);
          BSP_Ser_Printf("payload.msgType \n %x \n", payload.msgType);
          BSP_Ser_Printf("SOURCE NODE 4: TEMPERATURE MESSAGE \n Dew Point = %d Humidity = %u \n", payload.dataPart.hum.dewPt, payload.dataPart.hum.hum );

}
void PrintWind(Payload payload)
{
          CPU_INT08U BCD2 = (payload.dataPart.wind.speed[0] >> 4) & 0x0F;
          CPU_INT08U BCD1 = payload.dataPart.wind.speed[0] & 0x0F;
          CPU_INT08U BCD3 = (payload.dataPart.wind.speed[1] >> 4) & 0x0F;
          CPU_INT08U BCD4 = payload.dataPart.wind.speed[2] & 0x0F;
          CPU_INT16U reversed_wind = ((payload.dataPart.wind.dir & 0x00FF) << 8) | ((payload.dataPart.wind.dir & 0xFF00) >> 8);
          BSP_Ser_Printf("payload.payloadLen \n %x \n", payload.payloadLen);
          BSP_Ser_Printf("payload.dstAddr \n %x \n", payload.dstAddr);
          BSP_Ser_Printf("payload.srcAddr \n %x \n", payload.srcAddr);
          BSP_Ser_Printf("payload.msgType \n %x \n", payload.msgType);
          BSP_Ser_Printf("SOURCE NODE 5: WIND MESSAGE \n Speed = %d%d%d.%d Wind Direction = %u \n",BCD2,BCD1,BCD3,BCD4, reversed_wind );
}

void PrintRadiation(Payload payload)
{
          CPU_INT16U reversed_radiation = ((payload.dataPart.rad & 0x00FF) << 8) | ((payload.dataPart.rad & 0xFF00) >> 8);
          BSP_Ser_Printf("payload.payloadLen \n %x \n", payload.payloadLen);
          BSP_Ser_Printf("payload.dstAddr \n %x \n", payload.dstAddr);
          BSP_Ser_Printf("payload.srcAddr \n %x \n", payload.srcAddr);
          BSP_Ser_Printf("payload.msgType \n %x \n", payload.msgType);
          BSP_Ser_Printf("SOURCE NODE 6: SOLAR RADIATION MESSAGE \n Solar Radiation Intensity = %d \n", reversed_radiation);
}

void PrintDateTime(Payload payload)
{
          CPU_INT08U Hour =(((1 << 4) - 1) & (payload.dataPart.dateTime >> (0 - 1)));
          CPU_INT08U Minute = payload.dataPart.dateTime & 0x1F80;
          CPU_INT08U Year = payload.dataPart.dateTime >> 8;
          CPU_INT08U Month = payload.dataPart.dateTime >> 4;
          CPU_INT08U Day = payload.dataPart.dateTime >> 0;
          BSP_Ser_Printf("payload.payloadLen \n %x \n", payload.payloadLen);
          BSP_Ser_Printf("payload.dstAddr \n %x \n", payload.dstAddr);
          BSP_Ser_Printf("payload.srcAddr \n %x \n", payload.srcAddr);
          BSP_Ser_Printf("payload.msgType \n %x \n", payload.msgType);
          BSP_Ser_Printf("SOURCE NODE 7: DATE/TIME STAMP MESSAGE \n Hour = %d \n", Hour );
          BSP_Ser_Printf("SOURCE NODE 7: DATE/TIME STAMP MESSAGE \n Minute = %d \n", Minute );
          BSP_Ser_Printf("SOURCE NODE 7: DATE/TIME STAMP MESSAGE \n Year = %d \n", Year );
          BSP_Ser_Printf("SOURCE NODE 7: DATE/TIME STAMP MESSAGE \n Month = %d \n", Month);
          BSP_Ser_Printf("SOURCE NODE 7: DATE/TIME STAMP MESSAGE \n Day = %d \n", Day);
}
void PrintPrecipitation(Payload payload)
{
          CPU_INT08U BCD2 = (payload.dataPart.depth[0] >> 4) & 0x0F;
          CPU_INT08U BCD1 = payload.dataPart.depth[0] & 0x0F;
          CPU_INT08U BCD3 = (payload.dataPart.depth[1] >> 4) & 0x0F;
          CPU_INT08U BCD4 = payload.dataPart.depth[2] & 0x0F;
          BSP_Ser_Printf("SOURCE NODE 8: PRECIPITATION MESSAGE \n Speed = %d%d.%d%d",BCD2,BCD1,BCD3,BCD4 );

}
void PrintID(Payload payload)
{
         BSP_Ser_Printf("SOURCE NODE 9: Node ID = %c \n", payload.dataPart.id);
}