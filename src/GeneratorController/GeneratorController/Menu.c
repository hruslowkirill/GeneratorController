/*
 * Menu.c
 *
 * Created: 06.02.2017 13:44:25
 *  Author: kirill
 */ 

#include "menu.h"

uint8_t NUMBER_OF_SETTINGS(uint8_t RELAY)
{
	if (RELAY==PWM_RELAY)
		return 11;
	return 7;		
}

uint8_t NUMBER_OF_TERMS(uint8_t RELAY)
{
	if (RELAY==PWM_RELAY)
		return 2;
	return 3;		
}

void Menu_Init(Menu * menu, AllSettings * allSettings)
{
	menu->allSettings = allSettings;
}

void Menu_Reset(Menu * menu, MenuStatus ms)
{
	menu->ms = ms;
	menu->sub_current_status = 0;
	menu->current_relay = 0;
	menu->current_term = 0;
	menu->blinker = 0;
	
}

void Menu_Loop(Menu * menu)
{
	switch (menu->ms)
	{
		case MS_DEFAULT:
		break;

		case MS_SETTINGS:
			Menu_ShowSettings(menu);
		break; 
 
		case MS_RESET:
			Menu_ShowReset(menu);
		break;
	}			
}

void Menu_ShowReset(Menu * menu)
{
	LCD_2buffer_begin();
	LCD_2buffer_Move_Cursor(1);
	LCD_2buffer_printStr("RESET?????");

	LCD_2buffer_Move_Cursor(17);
	LCD_2buffer_printStr("No");
	
	LCD_2buffer_Move_Cursor(25);
	LCD_2buffer_printStr("Yes");

	switch(	menu->sub_current_status)
	{
		case 0:
			LCD_2buffer_Move_Cursor(16);
		break;

		case 1:
			LCD_2buffer_Move_Cursor(24);
		break;

	}
	
	LCD_2buffer_printStr(">");

	LCD_2buffer_end();

}

void Menu_ShowTerm(Menu * menu)
{
	LCD_2buffer_begin();
	LCD_2buffer_Move_Cursor(1);
	LCD_2buffer_printStr("Temp1");

	
	
	if (menu->current_relay!=PWM_RELAY)
	{
		LCD_2buffer_Move_Cursor(9);
		LCD_2buffer_printStr("Temp2");
		
		LCD_2buffer_Move_Cursor(17);
		LCD_2buffer_printStr("Temp3");
	}
	switch(	menu->sub_current_status)
	{
		case 0:
			LCD_2buffer_Move_Cursor(0);
		break;

		case 1:
			LCD_2buffer_Move_Cursor(8);
		break;

		case 2:
			LCD_2buffer_Move_Cursor(16);
		break;
	}
	LCD_2buffer_printStr(">");
	/*
	if (menu->current_relay==PWM_RELAY)
	{
		if (menu->allSettings->PWMSettings[0].on)
		{
			LCD_2buffer_Move_Cursor(6);		
			LCD_2buffer_printStr("*");
		}		
	}else
	{
		if (menu->allSettings->settings[menu->current_relay][0].on)
		{
			LCD_2buffer_Move_Cursor(6);		
			LCD_2buffer_printStr("*");
		}
		if (menu->allSettings->settings[menu->current_relay][1].on)
		{
			LCD_2buffer_Move_Cursor(14);		
			LCD_2buffer_printStr("*");
		}
		if (menu->allSettings->settings[menu->current_relay][2].on)
		{
			LCD_2buffer_Move_Cursor(22);		
			LCD_2buffer_printStr("*");
		}
	}	
	*/
	LCD_2buffer_end();
	//LCD_Clear_Display();
	//LCD_printStr("HELLO");
	//_delay_ms(1);
}

void Menu_ShowSettings(Menu * menu)
{
	
	menu->settings = &menu->allSettings->mainSettings;	

	menu->blinker = !menu->blinker;


	LCD_2buffer_begin();
	if (menu->settings_item1<=3)
	{
		MENU_SHOW_NUMBER(1, 1, "t33 ", menu->settings->t33)
		MENU_SHOW_NUMBER(2, 9, "t0 ", menu->settings->t0)
		MENU_SHOW_NUMBER(3, 17, "k ", menu->settings->k)
		MENU_SHOW_NUMBER(4, 25, "t3 ", menu->settings->t3)
		
	}
	else if (menu->settings_item1<=7)
	{
		MENU_SHOW_NUMBER(5, 1, "tn ", menu->settings->tn)
		MENU_SHOW_NUMBER(6, 9, "tp ", menu->settings->tp)  
		MENU_SHOW_NUMBER(7, 17, "ts ", menu->settings->tstop) 		
	}
	
	switch(	menu->settings_item1)
	{
		case 0:
			LCD_2buffer_Move_Cursor(0);
		break;

		case 1:
			LCD_2buffer_Move_Cursor(8);
		break;

		case 2:
			LCD_2buffer_Move_Cursor(16);
		break;
		
		case 3:
			LCD_2buffer_Move_Cursor(24);
		break;
		
		case 4:
			LCD_2buffer_Move_Cursor(0);
		break;
		
		case 5:
			LCD_2buffer_Move_Cursor(8);
		break;
		case 6:
			LCD_2buffer_Move_Cursor(16);
		break;
		case 7:
			LCD_2buffer_Move_Cursor(24);
		break;
		case 8:
			LCD_2buffer_Move_Cursor(0);
		break;
		case 9:
			LCD_2buffer_Move_Cursor(8);
		break;
		case 10:
			LCD_2buffer_Move_Cursor(16);
		break;
		}
		LCD_2buffer_printStr(">");	
	LCD_2buffer_end();
}



void Menu_ShowRelayMenu(Menu * menu)
{

	LCD_2buffer_begin();
	LCD_2buffer_Move_Cursor(1);
	LCD_2buffer_printStr("Relay1");

	LCD_2buffer_Move_Cursor(9);
	LCD_2buffer_printStr("Relay2");

	LCD_2buffer_Move_Cursor(17);
	LCD_2buffer_printStr("Relay3");
	
	LCD_2buffer_Move_Cursor(25);
	LCD_2buffer_printStr("Relay4");

	switch(	menu->sub_current_status)
	{
		case 0:
			LCD_2buffer_Move_Cursor(0);
		break;

		case 1:
			LCD_2buffer_Move_Cursor(8);
		break;

		case 2:
			LCD_2buffer_Move_Cursor(16);
		break;
		
		case 3:
			LCD_2buffer_Move_Cursor(24);
		break;
	}
	
	LCD_2buffer_printStr(">");

	LCD_2buffer_end();
	//LCD_Clear_Display();
	//LCD_printStr("HELLO");
	//_delay_ms(1);
}

void Menu_BtnMinusPressed(Menu * menu)
{
	switch(menu->ms)
	{
		case MS_DEFAULT:
		break;
		
		case MS_RESET:
			if(menu->sub_current_status==0)
				menu->sub_current_status = 2;
			menu->sub_current_status--;
		break;
		
		case MS_SETTINGS:
			if (menu->settings_item2==0)
			{
				if(menu->settings_item1==0)
					menu->settings_item1 = NUMBER_OF_SETTINGS(menu->current_relay);
				menu->settings_item1--;
			}

			Menu_Update_Settings_Normal(menu, -1, 0);
			
		break;
	}
}
void Menu_BtnPlusPressed(Menu * menu)
{

	switch(menu->ms)
	{
		case MS_DEFAULT:
		break;
		case MS_RESET:
		break;
		
		case MS_SETTINGS:
			if (menu->settings_item2==0)
			{	
				menu->settings_item1++;
				if(menu->settings_item1==NUMBER_OF_SETTINGS(menu->current_relay))
					menu->settings_item1 = 0;
			}

			Menu_Update_Settings_Normal(menu, 1, 0);
	
			
		break;
	}
}
void Menu_BtnOKPressed(Menu * menu)
{
	switch(menu->ms)
	{
		case MS_DEFAULT:
		break;

		
		case MS_RESET:
			Settings_Reset(menu->allSettings);
			//Menu_Reset(menu, MS_RELAY);
		break;
		

		case MS_SETTINGS:
			if (menu->settings_item2==0)
			{
				menu->settings_item2 = menu->settings_item1+1;
			}else
			{	
				menu->settings_item2 = 0;

				Settings_Write_Normal(menu->allSettings);
						
			}
		break;
	}
}

void Menu_BtnMinusLongPressed(Menu * menu)
{
	switch(menu->ms)
	{
		
		case MS_SETTINGS:


			Menu_Update_Settings_Normal(menu, -1, 1);

					
		break;
		case MS_DEFAULT:
		break;
		case MS_RESET:
		break;
	}
}

void Menu_BtnPlusLongPressed(Menu * menu)
{
	switch(menu->ms)
	{
		case MS_DEFAULT:
			Menu_switchToReset(menu);
		break;
		case MS_SETTINGS:

			Menu_Update_Settings_Normal(menu, 1, 1);


		break;
		case MS_RESET:
		break;
	}
}

void Menu_Update_Settings_Normal(Menu * menu, int8_t sign, uint8_t isLong)
{
	uint8_t mult = 1;
	
	if (isLong)
		mult = 10;
	
	if (menu->settings_item2==1)
	{
		menu->settings->t33+=sign*mult;
	}
	if (menu->settings_item2==2)
	{
		menu->settings->t0+=sign*mult;
	}
	if (menu->settings_item2==3)
	{
		menu->settings->k+=sign*mult;
	}
	if (menu->settings_item2==4)
	{
		menu->settings->t3+=sign*mult;
	}
	if (menu->settings_item2==5)
	{
		menu->settings->tn+=sign*mult;
	}
	if (menu->settings_item2==6)
	{
		menu->settings->tp+=sign*mult;
	}
	if (menu->settings_item2==7)
	{
		menu->settings->tstop+=sign*mult;
	}
	
}

void Menu_switchToDefault(Menu * menu)
{
	LCD_Clear_Display();
	menu->ms=MS_DEFAULT;
	menu->sub_current_status = 0;
}	


void Menu_switchToSettings(Menu * menu, uint8_t r, uint8_t t)
{
	menu->ms = MS_SETTINGS;
	menu->sub_current_status = 0;
	menu->current_relay = r;
	menu->current_term = t;
	menu->settings_item1 = 0;
	menu->settings_item2 = 0;
}

void Menu_switchToReset(Menu * menu)
{
	menu->ms = MS_RESET;
	menu->sub_current_status = 0;
	menu->settings_item1 = 0;
	menu->settings_item2 = 0;
}