#ifndef DEFINES_H
#define DEFINES_H

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define byte unsigned char



#define Off(PORT, N) PORT &= ~(1<<N)
#define On(PORT, N) PORT |= 1<<N

#define isOn(PIN, N) (PIN>>N)&1

#define delay_sec(S) for (byte i=0; i<S; i++) _delay_ms(1000)

#endif
