/*
 * GeneratorController.c
 *
 * Created: 27.10.2019 13:11:00
 *  Author: Кирилл
 */ 


#include "defines.h"
#include "lcd.h"
#include "menu.h"
#include "io_ports.h"

#define BTNDDR DDRC
#define BTNPORT PORTC
#define BTNPIN PINC

#define BTN1 0
#define BTN2 2
#define BTN3 1
#define IS_BTN_PRESSED(BTN)  ((BTNPIN & (1<<BTN))==0)

#define BTN_long_time 1000

typedef enum {PS_WORK, PS_MENU} ProgramState;
	
/*
WS_INIT - Initialize ports
WS_NETCHECK - CHECK IS NETWORK IS ON
WS_GEN_COOLDOWN - 
WS_NET_WAIT - 
WS_GEN_START - 
*/
typedef enum { WS_INIT, WS_NETCHECK, WS_GEN_COOLDOWN, WS_NET_WAIT, WS_GEN_START, WS_GEN_SWITCH } WorkState;
	


AllSettings allSettings;
ProgramState ps;
WorkState ws;
Menu menu;
uint32_t iterations = 0;
byte fault;

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
	DDRB = 0x00;
	LCD_Init();
	BTN_Init();
	Menu_Init(&menu, &allSettings);
	Settings_Init(&allSettings);
	io_init();
	
	ws = WS_INIT;
	
	ledON;
	
	/*
	while(1)
	{
		outOn(P1);
		delay_sec(5);
		outOff(P1);	
		delay_sec(5);	
	}
	return 0;*/
	
	
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

void printStatus(WorkState ws)
{
	switch (ws)
	{
		case WS_INIT:
			LCD_2buffer_printStr("WS_INIT");
		break;
		
		case WS_NETCHECK:
			LCD_2buffer_printStr("WS_NETCHECK");
		break;
		
		case WS_GEN_COOLDOWN:
			LCD_2buffer_printStr("WS_GEN_COOLDOWN");
		break;
		
		case WS_NET_WAIT:
			LCD_2buffer_printStr("WS_NET_WAIT");
		break;
		
		case WS_GEN_START:
			LCD_2buffer_printStr("WS_GEN_START");
		break;
		
		case WS_GEN_SWITCH:
			LCD_2buffer_printStr("WS_GEN_SWITCH");
		break;
	}		
}

void DoWork()
{
	byte t;
	byte x;
	if (iterations%100!=0)
		{
			iterations++;
			//_delay_ms(1);
			return;
		}
		
		
	switch (ws)
	{
		case WS_INIT:
			fault = 0;
			io_init();
			ws = WS_NETCHECK;
		break;
		
		case WS_NETCHECK:
			if (isNet())
			{
				outOff(P2);	
				delay_sec(allSettings.mainSettings.t0);
				outOn(P1);	
				if (isP(P3))
				{
					ws = WS_GEN_COOLDOWN;				
				}else
				{
					ws = WS_NETCHECK;
				}			
			}else
			{
				ws = WS_NET_WAIT;			
			}
		break;
		
		case WS_GEN_COOLDOWN:
			for (t=0; t<allSettings.mainSettings.tstop; t++)
			{
				delay_sec(1);
				if (isNet()==0)
				{
					//outOff(P3);
					break;
				}				
			}
			if (isNet())
			{
				outOff(P3);					
			}
			ws = WS_NETCHECK;
		break;
		
		case WS_NET_WAIT:
			if (fault)
			{
				ws = WS_NETCHECK;				
			}else
			{
				if (isGen())
				{
					ws = WS_GEN_SWITCH;				
				}else
				{
					for (t=0; t<allSettings.mainSettings.t33; t++)
					{
						delay_sec(1);
						if (isNet())	
						{
							break;
						}			
					}
					if (isNet())
					{
						ws = WS_NETCHECK;						
					}else
					{
						ws = WS_GEN_START;
					}
				}
			}
		break;
		
		case WS_GEN_START:
			outOn(P3);
			for (x=0; x<allSettings.mainSettings.k; x++)
			{
				delay_sec(allSettings.mainSettings.t0);
				outOn(P5);
				delay_sec(allSettings.mainSettings.t0);
				outOff(P5);		
				outOn(P4);	
				delay_sec(allSettings.mainSettings.t3+x);	
				outOff(P4);	
				delay_sec(allSettings.mainSettings.t0);
				outOn(P6);
				delay_sec(allSettings.mainSettings.t0);
				outOff(P6);
				delay_sec(allSettings.mainSettings.tn);
				if (isGen())
				{
					break;					
				}
			}
			if (isGen())
			{
				delay_sec(allSettings.mainSettings.tp);
				if (isNet())
				{
					ws = WS_NETCHECK;						
				}else
				{
					ws = WS_GEN_SWITCH;	
				}
			}else
			{
				fault = 1;
				ws = WS_NETCHECK;
			}
		break;
		
		case WS_GEN_SWITCH:
			outOff(P1);
			delay_sec(allSettings.mainSettings.t0);
			outOn(P2);
			ws = WS_NETCHECK;
		break;
		
	
	}
		
	LCD_2buffer_begin();
	printStatus(ws);
	LCD_2buffer_Move_Cursor(16);
	if (fault)
	{
		LCD_2buffer_printStr("FAULT");
	}	
	
	LCD_2buffer_Move_Cursor(22);
	if (isP(P1))
	{
		LCD_2buffer_printStr("NET");
	}	
	

	LCD_2buffer_Move_Cursor(26);
	if (isP(P2))
	{
		LCD_2buffer_printStr("GEN");	
	}
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