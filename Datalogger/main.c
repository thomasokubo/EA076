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
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"


// Variaveis utilizadas na leitura da temperatura
uint16_t tempValue, voltageValue, zero = 0;

// Valor do char a ser recebido pelo terminal, o array que 
// servira de buffer para armazenar uma determinada  mensagem 
// enviada do terminal e o buffer da memoria, 
unsigned char c, stringaoArrasador[1000], memoryBuffer[16], bufferIndex = 0, memoryIndex = 2;

// A variavel ind aponta para a posicao atual da mensagem enviada pelo terminal
unsigned int ind = 0, k, numCode;

// Funcao que calcula a temperatura ambiente, 
// a partir da tensao lida pelo conversor AD.
void ReadTemperature(){
	TSENSOR_Measure(TRUE);
	TSENSOR_GetValue16(&voltageValue);
	tempValue = (voltageValue*3300)/65535;
	tempValue = (tempValue-600)/10;	
}

void BufferWriteMemory(unsigned char mustWrite) {
	memoryBuffer[bufferIndex++] = tempValue;
	
	if(bufferIndex == 16 || mustWrite == 1) {
		if(memoryIndex + bufferIndex > 2046) {
			memoryIndex = 2;
		}
		// Grava valor da medição na memoria
		EE241_WriteBlock(memoryIndex, memoryBuffer, bufferIndex);	
		
		// Atualiza quantidade de valores salvos na memoria
		memoryIndex += bufferIndex;
		
		// Grava novo valor da posição da memoria
		EE241_WriteBlock(0, memoryIndex, 2);	

		bufferIndex = 0;
	}
}

// Funcao que realiza a leitura da temperatura ambiente e 
// escreve seu valor num  determinado espaco da memoria
void ReadAndWriteBufferTemperature(unsigned char mustWrite) {
	// Le valor de temperatura
	ReadTemperature();
	
	// Write to Memory
	BufferWriteMemory(1);
}

// Funcao que retorna o valor na n-esima posicao da memoria
char* GetN(uint16_t pos) {
	unsigned char dataString[3], data;

	if(pos > 0 && (pos + 2 <= memoryIndex + bufferIndex)) {
		if(pos + 2 <= memoryIndex) {
			EE241_ReadBlock(pos+2, &data, 1);
		} else {
			PARSER_xatoi(memoryBuffer[((pos+2)%memoryIndex)-1], &tempValue);
		}
		
		PARSER_Num16uToStr(dataString, 3, tempValue);
	}
	
	return dataString;
}

int main(void) {

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  AS1_Enable();
  
  for(;;){
	  
	  // Seta a posicao inicial da memoria com 0
	  EE241_WriteBlock(0, &zero, 2);
	  
	  // Verifica se recebeu um caractere do terminal
	  while(AS1_RecvChar(&c));
	  // Responde com o mesmo caractere, para que o usuário visualize
	  // o texto digitado
	  while(AS1_SendChar(c));
	  
	  // Caso tenha sido digitada a tecla enter,
	  // será feito o parsing da string digitada no terminal
	  if(c == '\r') {
		  // Adiciona \n ao final do array e remove o final que nao
		  // pertence a mensagem
		  stringaoArrasador[ind] = '\n';
		  while(AS1_SendChar('\n'));
		  PARSER_strCutTail(stringaoArrasador, stringaoArrasador+ind);
		  
		  // Caso a mensagem seja PING, responde-se com a mensagem PONG
		  if(!PARSER_strcmp(stringaoArrasador, "PING")){
			  char temp[] = "PONG";
			  for(k=0;k<4;k++)
				  while(AS1_SendChar(temp[k]));
			  while(AS1_SendChar('\n'));
			  while(AS1_SendChar('\r'));
			  
		  // Caso a mensagem seja ID, responde-se com uma string de identificação
		  } else if(!PARSER_strcmp(stringaoArrasador, "ID")){
			  char temp[] = "EA076_2017:Thomas_Yuri_Arrasadores_do_Universo";
			  for(k=0;k<PARSER_strlen(temp);k++)
				  while(AS1_SendChar(temp[k]));
			  while(AS1_SendChar('\n'));
			  while(AS1_SendChar('\r'));
			  
		  // Caso a mensagem seja MEASURE, responde-se com a temperatura atual do ambiente	  
		  } else if(!PARSER_strcmp(stringaoArrasador, "MEASURE")){
			  unsigned char dataString[3];
			  ReadTemperature();
			  PARSER_Num16uToStr(dataString, 3, tempValue);
			  for(k=0;k<PARSER_strlen(dataString);k++)
				  while(AS1_SendChar(dataString[k]));
			  while(AS1_SendChar('\n'));
			  while(AS1_SendChar('\r'));
			  
		  // Caso a mensagem seja MEMSTATUS, responde-se com o numero de elementos 
		  // na memoria
		  } else if(!PARSER_strcmp(stringaoArrasador, "MEMSTATUS")){
			  unsigned char iMemoryString[3];
			  
			  PARSER_Num16uToStr(iMemoryString, 3, memoryIndex + bufferIndex-2);
			  for(k=0;k<PARSER_strlen(iMemoryString);k++)
				  while(AS1_SendChar(iMemoryString[k]));
			  while(AS1_SendChar('\n'));
			  while(AS1_SendChar('\r'));
			 
		  // Caso a mensagem seja RECORD, nao apenas faz a leitura da temperatura,
		  // mas tambem escreve esse valor na memoria
		  } else if(!PARSER_strcmp(stringaoArrasador, "RECORD")){
			  ReadAndWriteBufferTemperature(1);

		  // Caso a mensagem seja RESET, muda-se o apontador da memoria para zero
		  } else if(!PARSER_strcmp(stringaoArrasador, "RESET")){
			  memoryIndex = 2;
			  bufferIndex = 0;
			  // Inicando a posicao inicial da memoria com 0
			  EE241_WriteBlock(0, &zero, 2);
			  
		  // Caso a mensagem seja GET N, retorna-se o n-esimo elemento da
		  // memoria
		  } else if(PARSER_strFind(stringaoArrasador, "GET ") == 0 && ind > 4) {
			  unsigned char *pos = stringaoArrasador+4;
			  char *dataStr;
			  unsigned int iMemory = 0, data;
			  stringaoArrasador[ind] = '\0';
			  PARSER_xatoi(&pos, &numCode);
			  
			  // Retorna o N-esimo elemento da memoria
			  dataStr = GetN(numCode);
			  
			  for(k=0;k<PARSER_strlen(dataStr);k++)
				  while(AS1_SendChar(*(dataStr+k)));
			  while(AS1_SendChar('\n'));
			  while(AS1_SendChar('\r'));
			  
			 /* 		  
	 	 	 // Le tamanho do vetor de dados da memoria EEPROM
			  EE241_ReadBlock(0, &iMemory, 4);
	
			  if(numCode <= iMemory && numCode >= 1) {
				  EE241_ReadBlock((numCode + 1)*4, &data, 4);
				  PARSER_Num32uToStr(dataString, 3,data);
				  for(k=0;k<PARSER_strlen(dataString);k++)
					  while(AS1_SendChar(dataString[k]));
				  while(AS1_SendChar('\n'));
				  while(AS1_SendChar('\r'));
			  }
			  */			
		  }
		  
		  // Retorna o indicador de posicao do stringaoArrasador para zero
		  // indicando a leitura de uma nova mensagem
		  ind = 0;
		  
	  // Caso o caractere nao seja enter, apenas o alocamos no array stringaoArrasador
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
