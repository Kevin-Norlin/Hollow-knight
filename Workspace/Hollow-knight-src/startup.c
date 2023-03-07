/*
 * 	startup.c
 *
 */
 
#include "objects.h"
#include "graphics.h"
#include "keyb.h"
#include "ascii.h"
#include "delay.h"
#include "timer6.h"
#include "logic.h"

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#define GPIO_D 0x40020C00
	#define GPIO_D_MODER ((volatile unsigned int *)(GPIO_D))
	#define GPIO_D_OTYPER ((volatile unsigned short *)(GPIO_D + 0x4))
	#define GPIO_D_SPEEDR ((volatile unsigned int *)(GPIO_D + 0x8))
	#define GPIO_D_PUPDR ((volatile unsigned int *)(GPIO_D + 0xC))
	#define GPIO_D_IDR_LOW ((volatile unsigned char *)(GPIO_D + 0x10))
	#define GPIO_D_IDR_HIGH ((volatile unsigned char *)(GPIO_D + 0x11))
	#define GPIO_D_ODR_LOW ((volatile unsigned char *)(GPIO_D + 0x14))
	#define GPIO_D_ODR_HIGH ((volatile unsigned char *)(GPIO_D + 0x15))

	
	#define GPIO_E 0x40021000
	#define GPIO_E_MODER ((volatile unsigned int *)(GPIO_E))
	#define GPIO_E_OTYPER ((volatile unsigned short *)(GPIO_E + 0x4))
	#define GPIO_E_SPEEDR ((volatile unsigned int *)(GPIO_E + 0x8))
	#define GPIO_E_PUPDR ((volatile unsigned int *)(GPIO_E + 0xC))
	#define GPIO_E_IDR_LOW ((volatile unsigned char *)(GPIO_E + 0x10))
	#define GPIO_E_IDR_HIGH ((volatile unsigned char *)(GPIO_E + 0x11))
	#define GPIO_E_ODR_LOW ((volatile unsigned char *)(GPIO_E + 0x14))
	#define GPIO_E_ODR_HIGH ((volatile unsigned char *)(GPIO_E + 0x15))	
	

void app_init(void);


void main(void)
{
	graphic_initialize();
	graphic_clear_screen();
	app_init();
	char prevSeconds;
	char c;
	int ballCount = 1;
	int collision = 0;
	OBJECT player = playerObject;
	POBJECT pplayer = &player;
	POBJECT balls[10];
	char str[10];
	OBJECT ball1 = ballobject;
	balls[0] = &ball1;
	
	
	
	while (1) {
		intToString(seconds, &str);
		
		
		if (seconds != prevSeconds) {
		ascii_clear_display();
		ascii_write_string(str);
		}
		prevSeconds = seconds;
		pplayer->move(pplayer);
		
		for (int i = 0; i < ballCount; i++) {
			balls[i]->move(balls[i]);
		}
	
		
		if (seconds == 100) {
			OBJECT ball2 = ballobject;
			
			balls[ballCount++] = &ball2;
		}
		if (seconds == 150) {
			OBJECT ball3 = ballobject;
			
			balls[ballCount++] = &ball3;
		}
		
		for (int i = 0; i < ballCount; i++) {
			if (check_collision(balls[i],pplayer)) {
				collision = 1;
			}
		}
		if (collision) {
			break;
		}
		
		
		handle_jump(pplayer);
		int speed = pplayer->diry;
		c = keyb();
		switch(c) {
			case 4: pplayer->set_speed(pplayer, -2, pplayer->diry); break;
			case 6: pplayer->set_speed(pplayer,  2, pplayer->diry); break;
			case 2: if (pplayer->jumpFrames == 0) { pplayer->set_speed(pplayer,pplayer->dirx,-2); pplayer->jumpFrames=40; break; }
			default: pplayer->set_speed(pplayer,0,pplayer->diry); break;
		}
	}
	
	
	ascii_clear_display();
	ascii_gotoxy(1,1);
	ascii_write_string("Game Over!");
	ascii_gotoxy(1,2);
	ascii_write_string("Score: ");
	ascii_write_string(str);
	 
	
}

void app_init(void) {
	
	// port D medium speed
	*((volatile unsigned int *)0x40020C08) = 0x55555555;
	/* starta klockor port D och E */
	* ( (unsigned long *) 0x40023830) = 0x18;
	
	// Keypad - DP8-15
	// 2 pin output, 2 pin input, 4 pin output
    *GPIO_D_MODER = 0x55000000;
	// Open drain
    *GPIO_D_OTYPER = 0xFF;
	// 2 pin Floating - 2 pin pull down - 4 pin reserved 
    *GPIO_D_PUPDR = 0xAAFFFF;
	
	timer6_init();
	
	*GPIO_E_MODER |= 0x00005555;
}



























	

	


