#include "settings.h"

AllSettings EEMEM EEMEMsettings = {
	INIT_NRLSET
};



void Settings_Init(AllSettings * allSettings)
{
	Settings_Read_Normal(allSettings);		
}
void Settings_Read_Normal(AllSettings * allSettings)
{
	eeprom_read_block (( void *) &allSettings->mainSettings , ( const void *) &EEMEMsettings.mainSettings , sizeof(Settings)) ;
}

void Settings_Write_Normal(AllSettings * allSettings)
{
	eeprom_update_block (( void *) &allSettings->mainSettings , ( const void *) &EEMEMsettings.mainSettings , sizeof(Settings)) ;
}

void Settings_Reset(AllSettings * allSettings)
{
	allSettings->mainSettings.t33 = DEFAULT_T33;
	allSettings->mainSettings.t0 = DEFAULT_T0;
	allSettings->mainSettings.k = DEFAULT_K;
	allSettings->mainSettings.t3 = DEFAULT_T3;
	allSettings->mainSettings.tn = DEFAULT_TN;
	allSettings->mainSettings.tp = DEFAULT_TP;
	allSettings->mainSettings.tstop = DEFAULT_TSTOP;
	Settings_Write_Normal(allSettings);
	
}



 
