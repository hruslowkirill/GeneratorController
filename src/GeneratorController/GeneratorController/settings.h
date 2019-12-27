#ifndef SETTINGS_H
#define SETTINGS_H

#include "defines.h"
#include <avr/eeprom.h>
#define DEFAULT_T33 60
#define DEFAULT_T0 1
#define DEFAULT_K 3
#define DEFAULT_T3 2
#define DEFAULT_TN 2
#define DEFAULT_TP 60
#define DEFAULT_TSTOP 60


#define INIT_NRLSET {DEFAULT_T33, DEFAULT_T0, DEFAULT_K, DEFAULT_T3, DEFAULT_TN, DEFAULT_TP, DEFAULT_TSTOP}


typedef struct
{
	uint8_t t33;
	uint8_t t0;
	uint8_t k;
	uint8_t t3;
	uint8_t tn;
	uint8_t tp;
	uint8_t tstop;	
}Settings; //7


typedef struct  
{
	Settings mainSettings;
}AllSettings;
 
void Settings_Init(AllSettings * allSettings);
void Settings_Read_Normal(AllSettings * allSettings);
void Settings_Write_Normal(AllSettings * allSettings);
void Settings_Reset(AllSettings * allSettings);





#endif
