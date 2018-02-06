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
#pragma pack(1) //Don't align on word boundaries
/* Include Micrium and STM headers. */
#include "includes.h"



/*----- c o n s t a n t    d e f i n i t i o n s -----*/

#define BaudRate 9600           /* RS232 Port Baud Rate */
  
/*----- f u n c t i o n    p r o t o t y p e s -----*/

int AppMain();

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
      CPU_INT08U    id[10]    //don't use hard coded
    } dataPart;
} Payload;


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

 BSP_Ser_Printf("\nsPROGRAM STARTED...\n");

  Payload payload;
  

  for(;;)
  {
      ParsePkt(&payload);

      switch(payload.msgType)
      {
        case 1: //temprature
          BSP_Ser_Printf("payload.payloadLen \n %x \n", payload.payloadLen);
          BSP_Ser_Printf("payload.dstAddr \n %x \n", payload.dstAddr);
          BSP_Ser_Printf("payload.srcAddr \n %x \n", payload.srcAddr);
          BSP_Ser_Printf("payload.msgType \n %x \n", payload.msgType);
          BSP_Ser_Printf("SOURCE NODE 2: TEMPERATURE MESSAGE \n Temperature = %d \n", payload.dataPart.temp);
          break;
        case 2: //pressure
          CPU_INT16U reversed_pressure = ((payload.dataPart.pres & 0x00FF) << 8) | ((payload.dataPart.pres & 0xFF00) >> 8);
          BSP_Ser_Printf("payload.payloadLen \n %x \n", payload.payloadLen);
          BSP_Ser_Printf("payload.dstAddr \n %x \n", payload.dstAddr);
          BSP_Ser_Printf("payload.srcAddr \n %x \n", payload.srcAddr);
          BSP_Ser_Printf("payload.msgType \n %x \n", payload.msgType);
          BSP_Ser_Printf("SOURCE NODE 3: BAROMETRIC PRESSURE MESSAGE \n Pressure = %d \n", reversed_pressure);
          break;
         case 3: //humidity
          BSP_Ser_Printf("payload.payloadLen \n %x \n", payload.payloadLen);
          BSP_Ser_Printf("payload.dstAddr \n %x \n", payload.dstAddr);
          BSP_Ser_Printf("payload.srcAddr \n %x \n", payload.srcAddr);
          BSP_Ser_Printf("payload.msgType \n %x \n", payload.msgType);
          BSP_Ser_Printf("SOURCE NODE 4: TEMPERATURE MESSAGE \n Dew Point = %d Humidity = %u \n", payload.dataPart.hum.dewPt, payload.dataPart.hum.hum );
          break;
         case 4: //wind
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
          break;
         case 5: //wind

          CPU_INT16U reversed_radiation = ((payload.dataPart.rad & 0x00FF) << 8) | ((payload.dataPart.rad & 0xFF00) >> 8);
          BSP_Ser_Printf("payload.payloadLen \n %x \n", payload.payloadLen);
          BSP_Ser_Printf("payload.dstAddr \n %x \n", payload.dstAddr);
          BSP_Ser_Printf("payload.srcAddr \n %x \n", payload.srcAddr);
          BSP_Ser_Printf("payload.msgType \n %x \n", payload.msgType);
          BSP_Ser_Printf("SOURCE NODE 6: SOLAR RADIATION MESSAGE \n Solar Radiation Intensity = %d \n", reversed_radiation);
          break;

      }
  } 
  


  
  return 0;
}