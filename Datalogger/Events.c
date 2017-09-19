/* ###################################################################
**     Filename    : Events.c
**     Project     : lab02
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-09-05, 16:32, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 

int pos = 0;
char stringao[3], aux;

/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TSENSOR_OnEnd (module Events)
**
**     Component   :  TSENSOR [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TSENSOR_OnEnd(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TSENSOR_OnCalibrationEnd (module Events)
**
**     Component   :  TSENSOR [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TSENSOR_OnCalibrationEnd(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  C3_OnInterrupt (module Events)
**
**     Component   :  C3 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void C3_OnInterrupt(void)
{
	  C3_Disable();
	  
	  // Deteccao do aperto de uma tecla pertecente a terceira coluna
	  int i;
	  WAIT1_Waitms(3);
	  
	  // Configura o restante das linhas como modo de entrada
	  L2_SetInput();
	  L3_SetInput();
	  L4_SetInput();
	  
	  //WAIT1_Waitms(3);
	  WAIT1_Waitus(100);  
	  // Realiza varrudura ate identificar a tecla pressionada
	  for(i = 0; i < 4; i++) {
		// Quando o valor de C1 for zero, reconheceu-se a tecla
		if(!C3_GetVal()) {
		  // O valor do teclado a ser atribuído depende da iteração corrente
		  			
		  if(i==0){
			stringao[pos++] = '3';
			aux  = '3';
		  } else if(i==1){
			stringao[pos++] = '6';
			aux = '6';
		  } else if(i==2){
			stringao[pos++] = '9';
			aux = '9';
		  } else {
			stringao[pos++] = '*';
			aux = '*';
			if(stringao[(pos)%3]== '#'){
				switch(stringao[(pos+1)%3]){
					case '1': // Pisca um LED, indicando que o sistema esta responsivo.
						LED_PutVal(1);
						break;
					case '2': // Realiza uma medicao e grava o valor na mem´oria.
						
						break;
					case '3': // Ativa modo de medicao automatica.
						
						break;
					case '4': // Encerra modo de medicao automatica.
						
						break;
					default:
						break;
				}
			}
			
		  }
		  i=10;
		  pos = pos % 3;
		} else {
		  if(i==0)
		    L2_SetOutput();
		  else if(i==1)
		    L3_SetOutput();
		  else
			L4_SetOutput();
		}
		
		WAIT1_Waitus(100);	
	  }
	  
	  // Reconfigurar as linhas
	  L2_SetOutput();
	  L3_SetOutput();
	  L4_SetOutput();
	  
	  WAIT1_Waitus(100);
	    
	  C3_Enable();
}

/*
** ===================================================================
**     Event       :  C2_OnInterrupt (module Events)
**
**     Component   :  C2 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void C2_OnInterrupt(void)
{
	  C2_Disable();
	
	  // Deteccao do aperto de uma tecla pertecente a segunda coluna
	  int i;
	  WAIT1_Waitms(3);
	  
	  // Configura o restante das linhas como modo de entrada
	  L2_SetInput();
	  L3_SetInput();
	  L4_SetInput();
	  
	  //WAIT1_Waitms(3);
	  WAIT1_Waitus(100);  
	  // Realiza varrudura ate identificar a tecla pressionada
	  for(i = 0; i < 4; i++) {
		// Quando o valor de C1 for zero, reconheceu-se a tecla
		if(!C2_GetVal()) {
		  // O valor do teclado a ser atribuído depende da iteração corrente
		  if(i==0){
			stringao[pos++] = '2';
			aux  = '2';
		  } else if(i==1){
			stringao[pos++] = '5';
			aux = '5';
		  } else if(i==2){
			stringao[pos++] = '8';
			aux = '8';
		  } else {
			stringao[pos++] = '0';
			aux = '0';
		  }
		  i=10;
		  pos = pos % 3;
		} else {
		  if(i==0)
		    L2_SetOutput();
		  else if(i==1)
		    L3_SetOutput();
		  else
			L4_SetOutput();
		}
		
		WAIT1_Waitus(100);	
	  }
	  
	  // Reconfigurar as linhas
	  L2_SetOutput();
	  L3_SetOutput();
	  L4_SetOutput();
	  
	  WAIT1_Waitus(100);
	  
	  C2_Enable();
}

/*
** ===================================================================
**     Event       :  C1_OnInterrupt (module Events)
**
**     Component   :  C1 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void C1_OnInterrupt(void)
{
  C1_Disable();
  
  // Deteccao do aperto de uma tecla pertecente a primeira coluna
  int i;
  WAIT1_Waitms(10);
  
  // Configura o restante das linhas como modo de entrada
  L2_SetInput();
  L3_SetInput();
  L4_SetInput();
  
  //WAIT1_Waitms(3);
  WAIT1_Waitus(100);  
  // Realiza varrudura ate identificar a tecla pressionada
  for(i = 0; i < 4; i++) {
	// Quando o valor de C1 for zero, reconheceu-se a tecla
	if(!C1_GetVal()) {
	  // O valor do teclado a ser atribuído depende da iteração corrente
	  if(i==0){
		stringao[pos++] = '1';
		aux  = '1';
	  } else if(i==1){
		stringao[pos++] = '4';
		aux = '4';
	  } else if(i==2){
		stringao[pos++] = '7';
		aux = '7';
	  } else {
		stringao[pos++] = '#';
		aux = '#';
	  }
	  i=10;
	  pos = pos % 3;
	} else {
	  if(i==0)
	    L2_SetOutput();
	  else if(i==1)
	    L3_SetOutput();
	  else
		L4_SetOutput();
	}
	
	WAIT1_Waitus(100);	
  }
  
  // Reconfigurar as linhas
  L2_SetOutput();
  L3_SetOutput();
  L4_SetOutput();
  
  WAIT1_Waitus(100);
  
  C1_Enable();
  
 }

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
