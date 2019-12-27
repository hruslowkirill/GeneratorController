/*
 * io_ports.h
 *
 * Created: 25.12.2019 3:54:57
 *  Author: Кирилл
 */ 


#ifndef IO_PORTS_H_
#define IO_PORTS_H_

#include <avr/io.h>

#include "defines.h"

#define ODDR DDRB
#define OPORT PORTB
#define OPIN PINB

#define IDDR DDRC
#define IPORT PORTC
#define IPIN PINC

#define P1 0
#define P2 1
#define P3 2
#define P4 3
#define P5 4
#define P6 5

#define Nnet 3
#define Ngen 4

void io_init();
void io_output_off();

byte isNet();
byte isGen();
byte isP(byte P);

void outOn(byte P);
void outOff(byte P);

#endif /* IO_PORTS_H_ */