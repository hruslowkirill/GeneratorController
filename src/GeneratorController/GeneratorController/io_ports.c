/*
 * io_ports.c
 *
 * Created: 25.12.2019 3:55:05
 *  Author: Кирилл
 */ 
#include "io_ports.h"

void io_init()
{
	On(ODDR, P1);
	On(ODDR, P2);
	On(ODDR, P3);
	On(ODDR, P4);
	On(ODDR, P5);
	On(ODDR, P6);
	
	io_output_off();
	
	Off(IDDR, Nnet);
	Off(IDDR, Ngen);
}

void io_output_off()
{
	Off(OPORT, P1);
	Off(OPORT, P2);
	Off(OPORT, P3);
	Off(OPORT, P4);
	Off(OPORT, P5);
	Off(OPORT, P6);
}

byte isNet()
{
	return (~isOn(IPIN, Nnet))&1;
}
byte isGen()
{
	return (~isOn(IPIN, Ngen))&1;
}

byte isP(byte P)
{
	return isOn(OPIN, P);
}

void outOn(byte P)
{
	On(OPORT, P);
}
void outOff(byte P)
{
	Off(OPORT, P);
}