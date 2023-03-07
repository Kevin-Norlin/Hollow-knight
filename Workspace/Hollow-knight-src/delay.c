#define SYS_TICK 0xE000E010
	#define STK_CTRL	((volatile unsigned int *) SYS_TICK)
	#define	STK_LOAD	((volatile unsigned int *) (SYS_TICK + 0x04))
	#define STK_VAL		((volatile unsigned int *) (SYS_TICK + 0x08))


// ---- Delay ----

void delay_250ns(void) {
	int countValue = 42; // Antal klockcykler för 250ns
	*STK_CTRL = 0;
	*STK_LOAD = countValue;
	*STK_VAL = 0;
	*STK_CTRL = 5;
	volatile int count = *STK_CTRL;
	while ((count < 30)){
		count = *STK_CTRL;
		}
	*STK_CTRL = 0;
}

void delay_mikro(unsigned int us) {
	for (int i = 0; i < us; i++) {
		delay_250ns();
		delay_250ns();
		delay_250ns();
		delay_250ns();
	}
}
void delay_milli(unsigned int ms) {
	for (int i = 0; i < ms; i++) {
		delay_mikro(1000);
	}
}