/* ###################################################################
**     Filename    : main.c
**     Project     : lab02
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-09-05, 16:32, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "TSENSOR.h"
#include "AdcLdd1.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "C1.h"
#include "ExtIntLdd1.h"
#include "C2.h"
#include "ExtIntLdd2.h"
#include "C3.h"
#include "ExtIntLdd3.h"
#include "L1.h"
#include "BitIoLdd1.h"
#include "L2.h"
#include "BitIoLdd3.h"
#include "L3.h"
#include "BitIoLdd2.h"
#include "L4.h"
#include "BitIoLdd4.h"
#include "EE241.h"
#include "WAIT1.h"
#include "GI2C1.h"
#include "KSDK1.h"
#include "CI2C1.h"
#include "LED.h"
#include "BitIoLdd5.h"
#include "PARSER.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
uint16_t tempValue, voltageValue;

// Funcao que realiza calcula a temperatura ambiente, 
// a partir da tensao lida pelo conversor AD.
void ReadTemperature(){
	TSENSOR_Measure(TRUE);
	TSENSOR_GetValue16(&voltageValue);
	tempValue = (voltageValue*3300)/65535;
	tempValue = (tempValue-600)/10;	
}

// Valor do char a ser recebido pelo terminal
unsigned char c, stringaoArrasador[1000];
unsigned int ind = 0, k;


int main(void) {

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  AS1_Enable();
  for(;;){
	  ReadTemperature();

	  while(AS1_RecvChar(&c));
	  while(AS1_SendChar(c));
	  // Caso tenha sido digitada a tecla enter
	  if(c == '\r') {
		  stringaoArrasador[ind] = '\n';
		  while(AS1_SendChar('\n'));
		  
		  if(ind == 4){
			  PARSER_strCutTail(stringaoArrasador, stringaoArrasador+ind);
			  if(!PARSER_strcmp(stringaoArrasador, "PING")){
				  char temp[] = "PONG";
				  for(k=0;k<4;k++)
					  while(AS1_SendChar(temp[k]));
				  while(AS1_SendChar('\n'));
				  while(AS1_SendChar('\r'));
			  }
			  
		  } else if(ind == 2){
			  PARSER_strCutTail(stringaoArrasador, stringaoArrasador+ind);
			  if(!PARSER_strcmp(stringaoArrasador, "ID")){
				  char temp[] = "SARRADA EH TOP!";
				  for(k=0;k<15;k++)
					  while(AS1_SendChar(temp[k]));
				  while(AS1_SendChar('\n'));
				  while(AS1_SendChar('\r'));
			  }
		  } else if(ind == 7) {
			  PARSER_strCutTail(stringaoArrasador, stringaoArrasador+ind);
			  if(!PARSER_strcmp(stringaoArrasador, "MEASURE")){
				  char temp[] = "PONG";
				  for(k=0;k<4;k++)
					  while(AS1_SendChar(temp[k]));
				  while(AS1_SendChar('\n'));
				  while(AS1_SendChar('\r'));
			  }
		  } else if(ind == 9){
			  PARSER_strCutTail(stringaoArrasador, stringaoArrasador+ind);
			  if(!PARSER_strcmp(stringaoArrasador, "MEMSTATUS")){
				  char temp[] = "PONG";
				  for(k=0;k<4;k++)
					  while(AS1_SendChar(temp[k]));
				  while(AS1_SendChar('\n'));
				  while(AS1_SendChar('\r'));
			  }
		  } else if(ind == 5){
			  PARSER_strCutTail(stringaoArrasador, stringaoArrasador+ind);
			  if(!PARSER_strcmp(stringaoArrasador, "RESET")){
				  char temp[] = "PONG";
				  for(k=0;k<4;k++)
					  while(AS1_SendChar(temp[k]));
				  while(AS1_SendChar('\n'));
				  while(AS1_SendChar('\r'));
			  } else if(!PARSER_strcmp(stringaoArrasador, "GET 5")){
				  char temp[] = "PONG";
				  for(k=0;k<4;k++)
					  while(AS1_SendChar(temp[k]));
				  while(AS1_SendChar('\n'));
				  while(AS1_SendChar('\r'));				  
			  }
		  } else if(ind == 6){
			  PARSER_strCutTail(stringaoArrasador, stringaoArrasador+ind);
			  if(!PARSER_strcmp(stringaoArrasador, "RECORD")){
				  char temp[] = "PONG";
				  for(k=0;k<4;k++)
					  while(AS1_SendChar(temp[k]));
				  while(AS1_SendChar('\n'));
				  while(AS1_SendChar('\r'));
			  }
		  }
		  ind = 0;
	  } else {
		  stringaoArrasador[ind++] = c;
	  }
	  
  }
  
  

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
