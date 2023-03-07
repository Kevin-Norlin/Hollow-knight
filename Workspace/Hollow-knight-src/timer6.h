#ifndef	TIMER6_H
#define	TIMER6_H

void timer6_interrupt();
void timer6_init();
volatile int ticks;
volatile int seconds;
#endif