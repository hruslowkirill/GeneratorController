/*
 * GeneratorController.c
 *
 * Created: 27.10.2019 13:11:00
 *  Author: Кирилл
 */ 


#include "defines.h"
#include "lcd.h"
#include "menu.h"

#define BTNDDR DDRC
#define BTNPORT PORTC
#define BTNPIN PINC

#define BTN1 0
#define BTN2 2
#define BTN3 1
#define IS_BTN_PRESSED(BTN)  ((BTNPIN & (1<<BTN))==0)

#define BTN_long_time 1000

#define LED_OFF_TIME 10000

typedef enum {PS_WORK, PS_MENU} ProgramState;

AllSettings allSettings;
ProgramState ps;
Menu menu;
uint32_t iterations = 0;

void ShowMenu(MenuStatus ms);
void HideMenu();
void DoWork();

void BTN_Init();
void BTN_Process();
void BTN1_Pressed();
void BTN2_Pressed();
void BTN3_Pressed();
void BTN3_Long_Pressed();

int32_t BTN1_status;
int32_t BTN2_status;
int32_t BTN3_status;

int main(void)
{
	DDRB = 0xFF;
	LCD_Init();
	BTN_Init();
	Menu_Init(&menu, &allSettings);
	
	ledON;
    while(1)
    {
		BTN_Process();
		_delay_ms(1);

		switch(ps)
		{
			case PS_WORK:
				
				DoWork();			
			break;
			case PS_MENU:
				if (iterations%300!=0)
				{
					iterations++;
				}else
				{
					Menu_Loop(&menu);
					iterations = 1;
				}				
			break;
		}
    }
}

void ShowMenu(MenuStatus ms)
{
	cli();
	Menu_Reset(&menu, ms);
	ps = PS_MENU;
	iterations = 0;
}

void HideMenu()
{
	ps = PS_WORK;
	sei();
}

void DoWork()
{
	if (iterations%500!=0)
		{
			iterations++;
			//_delay_ms(1);
			return;
		}
		
	LCD_2buffer_begin();
	LCD_2buffer_printStr("Hello");
	LCD_2buffer_end();
	iterations = 1;
	_delay_us(100);
}	


void BTN_Init()
{
	BTN1_status = 0;
	BTN2_status = 0;
	BTN3_status = 0;

	Off(BTNDDR, BTN1); 
	Off(BTNDDR, BTN2); 
	Off(BTNDDR, BTN3); 

	On(BTNPORT, BTN1); 
	On(BTNPORT, BTN2); 
	On(BTNPORT, BTN3); 
}

void BTN1_Pressed()
{
	Menu_BtnMinusPressed(&menu);
	iterations = 0;
}
void BTN2_Pressed()
{
	Menu_BtnPlusPressed(&menu);
	iterations = 0;
}
void BTN3_Pressed()
{
	Menu_BtnOKPressed(&menu);
	iterations = 0;
}
void BTN1_Long_Pressed()
{
	Menu_BtnMinusLongPressed(&menu);
	iterations = 0;
}

void BTN2_Long_Pressed()
{
	if (ps==PS_WORK)
	{
		ShowMenu(MS_RESET);	
	}else
	{
		Menu_BtnPlusLongPressed(&menu);
	}	
	iterations = 0;
}

void BTN3_Long_Pressed()
{
	if (ps==PS_WORK)
	{
		ShowMenu(MS_SETTINGS);		
	}else
	{
		HideMenu();
	}
	iterations=0;
}

void BTN_Process()
{
	//if ((BTNPIN & (1<<BTN1))==0)
	if (IS_BTN_PRESSED(BTN1))
	{
		if (BTN1_status>=0)
			BTN1_status ++;
		//Led_On_time = 0;
	}

	if (IS_BTN_PRESSED(BTN2))
	{
		if (BTN2_status>=0)
			BTN2_status ++;
		//Led_On_time = 0;
	}

	if (IS_BTN_PRESSED(BTN3))
	{
		if (BTN3_status>=0)
			BTN3_status ++;
		//Led_On_time = 0;
	}

	if (BTN1_status>BTN_long_time)
	{
		if (BTN1_status>=0)
			BTN1_Long_Pressed();
		BTN1_status = -1;
	}
	
	if (BTN2_status>BTN_long_time)
	{
		if (BTN2_status>=0)
			BTN2_Long_Pressed();
		BTN2_status = -1;
	}

	if (BTN3_status>BTN_long_time)
	{
		if (BTN3_status>=0)
			BTN3_Long_Pressed();
		BTN3_status = -1;
	}

	if (!IS_BTN_PRESSED(BTN1))
	{
		if ((BTN1_status>0)&&(BTN1_status<BTN_long_time))
		{
			BTN1_Pressed();
		}
		BTN1_status = 0;
	}

	if (!IS_BTN_PRESSED(BTN2))
	{
		if ((BTN2_status>0)&&(BTN2_status<BTN_long_time))
		{
			BTN2_Pressed();
		}
		BTN2_status = 0;
	}

	if (!IS_BTN_PRESSED(BTN3))
	{
		if ((BTN3_status>0)&&(BTN3_status<BTN_long_time))
		{
			BTN3_Pressed();
		}
		BTN3_status = 0;
	}
	
}