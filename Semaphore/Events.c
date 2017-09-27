/* ###################################################################
**     Filename    : Events.c
**     Project     : lab01
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-08-16, 09:15, # CodeGen: 0
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


extern volatile int state, timer;
extern volatile uint16_t LDRvalue;

int LDRcounter = 0;

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
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TI1_OnInterrupt(void)
{
	if(LDRvalue < 5000) {
		if(LDRcounter <= 10)
			LDRcounter++;
	} else {
		if(LDRcounter == 0) {
			if(state == 4)
				state = 0;
		} else {
			LDRcounter--;
		}
	}
	if(LDRcounter > 10){
		state = 4;
	}
	
	if(state==1){
		if(timer<8)timer++;
		else if(timer==8){
			timer = 0;
			state = 2;
		}
	} else if(state==2){
		if(timer<8) timer++;
		else if(timer==8){
			timer = 0;
			state = 3;
		}
	} else if(state==3){
		if(timer<8){
			timer++;
			RedPed_NegVal();
		} else if(timer == 8){
			timer = 0;
			state = 0;
		}
	} else if(state==4){
		YellowCar_NegVal();
		RedPed_NegVal();
	}

	
}

void Button_OnInterrupt(void) {
	if(state != 4) {
		state = 1;
		timer = 0;	
	}
}


/*
** ===================================================================
**     Event       :  LDR_OnEnd (module Events)
**
**     Component   :  LDR [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LDR_OnEnd(void)
{
  /* Write your code here ... */
	  
}

/*
** ===================================================================
**     Event       :  LDR_OnCalibrationEnd (module Events)
**
**     Component   :  LDR [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LDR_OnCalibrationEnd(void)
{
  /* Write your code here ... */
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
