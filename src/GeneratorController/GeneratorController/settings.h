#ifndef SETTINGS_H
#define SETTINGS_H

#include "defines.h"
#include <avr/eeprom.h>
#define DEFAULT_VALUE 1

#define DEFAULT_TEMP 20.5
#define DEFAULT_D 1.0
#define DEFAULT_DIRECTION 1
#define DEFAULT_ON 0

#define DEFAULT_TZAP 70
#define DEFAULT_T3 1
#define DEFAULT_TSTOP 0
#define DEFAULT_TSTART 0
#define DEFAULT_TSLIV 70
#define DEFAULT_TPWM 10
#define DEFAULT_SGSHIM 1
#define DEFAULT_SCSHIM 1 
#define DEFAULT_SSHIM 1
#define DEFAULT_SSDIMD 1

#define INIT_PWNSET {DEFAULT_ON, DEFAULT_TZAP, DEFAULT_T3, DEFAULT_TSTOP, DEFAULT_TSTART, DEFAULT_TSLIV, DEFAULT_TPWM, DEFAULT_SGSHIM, DEFAULT_SCSHIM, DEFAULT_SSHIM, DEFAULT_SSDIMD}
#define INIT_NRLSET {DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE}


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
