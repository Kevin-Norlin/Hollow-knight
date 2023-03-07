	
	/* Timer 6 */
	#define TIM6    0x40001000
	#define	TIM6_CR1	((volatile unsigned short *) 0x40001000)
	#define	TIM6_DIER	((volatile unsigned short *) 0x4000100C)
	#define	TIM6_SR 	((volatile unsigned short *) 0x40001010)
	#define	TIM6_CNT	((volatile unsigned short *) 0x40001024)
	#define TIM6_PSC    ((volatile unsigned short *) 0x40001028)
	#define	TIM6_ARR	((volatile unsigned short *) 0x4000102C)

	#define SCB_VTOR    ((volatile unsigned int *) 0xE000ED08) // Relokeras automatiskt --> 0x2001C000
	#define TIM6_IRQVEC ((volatile unsigned int *) 0x2001C118)

	#define NVIC_TIM6_IRQ_BPOS (1 << 22)
	#define NVIC_TIM6_ISER  ((volatile unsigned int*) 0xE000E104)
	#define UIE         (1 << 0) // 01 Binärt
	#define UIF         (1 << 0) 
	#define	UDIS		(1 << 1) // 10
	#define	CEN			(1 << 0) 
	

// ---- Global variables ----
volatile int ticks;
volatile int seconds;

// ---- Tim6 ----
void timer6_interrupt()
{
    //100 ms period
    *TIM6_SR &= ~UIF; // Kvitera avbrott
    ticks++;
    if (ticks > 9)
    {
        ticks = 0;
        seconds++;
    }
}

void timer6_init()
{
	
    ticks = 0;
    seconds = 0;
    *TIM6_CR1 &= ~CEN;
	
	
	*TIM6_IRQVEC = timer6_interrupt;
    *NVIC_TIM6_ISER |= NVIC_TIM6_IRQ_BPOS;
	//Funkar bra på hårdvaran, för segt här
    // 100 ms tidbas
    //*TIM6_PSC    = 839;
    //*TIM6_ARR   = 9999;
	*TIM6_PSC    = 114;
	*TIM6_ARR   = 999;
	
    *TIM6_DIER |= UIE;
    *TIM6_CR1  |= CEN;



}

