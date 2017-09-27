/* ###################################################################
**     Filename    : main.c
**     Project     : lab01
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Authors 	   : Thomas and Yuri
**     Date/Time   : 2017-08-16, 09:15, # CodeGen: 0
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
#include "LED.h"
#include "BitIoLdd1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "GreenCar.h"
#include "BitIoLdd2.h"
#include "YellowCar.h"
#include "BitIoLdd4.h"
#include "RedCar.h"
#include "BitIoLdd5.h"
#include "GreenPed.h"
#include "BitIoLdd3.h"
#include "RedPed.h"
#include "BitIoLdd6.h"
#include "Button.h"
#include "ExtIntLdd1.h"
#include "LDR.h"
#include "AdcLdd1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

volatile int state= 0;
volatile int timer=0;
volatile uint16_t LDRvalue;


void CarState(){
	GreenCar_PutVal(1);
	YellowCar_PutVal(0);
	RedCar_PutVal(0);
	GreenPed_PutVal(0);
	RedPed_PutVal(1);
}

void WaitState(){
	GreenCar_PutVal(0);
	YellowCar_PutVal(1);
	RedCar_PutVal(0);
	GreenPed_PutVal(0);
	RedPed_PutVal(1);	
}

void PedState(){
	GreenCar_PutVal(0);
	YellowCar_PutVal(0);
	RedCar_PutVal(1);
	GreenPed_PutVal(1);
	RedPed_PutVal(0);			
}

void AlarmState(){
	GreenCar_PutVal(0);
	YellowCar_PutVal(0);
	RedCar_PutVal(1);
	GreenPed_PutVal(0);	
}

void NightState(){
	GreenCar_PutVal(0);
	RedCar_PutVal(0);
	GreenPed_PutVal(0);
}


/* User includes (#include below this line is not maintained by Processor Expert) */

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  //for(;;){}
  for(;;){	  
	  (void)LDR_Measure(TRUE); /* do conversion and wait for the result */
	  (void)LDR_GetValue16(&LDRvalue); /* get the result into value variable */
	  
	  switch(state){
	  case 0:
		  CarState();
		  break;
		  
	  case 1:
		  WaitState();
		 
		  break;
		  
	  case 2:
		  PedState();
		  
		  break;
		  
	  case 3:
		  AlarmState();
		  break;
		  
	  case 4:
		  NightState();
		  break;
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
