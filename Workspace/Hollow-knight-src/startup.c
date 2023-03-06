/*
 * 	startup.c
 *
 */
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
	
	// Bitar i styrregistret ASCII
	#define	B_E			0x40
	#define B_SELECT 	4
	#define	B_RW		2
	#define	B_RS		1
	
#define SYS_TICK 0xE000E010
	#define STK_CTRL	((volatile unsigned int *) SYS_TICK)
	#define	STK_LOAD	((volatile unsigned int *) (SYS_TICK + 0x04))
	#define STK_VAL		((volatile unsigned int *) (SYS_TICK + 0x08))

	

	#define MAX_POINTS	50

// Structs

typedef struct {
	char x,y;
} PIXEL,*PPIXEL;


typedef struct {
	int numPixels;
	int sizex, sizey;
	PIXEL pixels[ MAX_POINTS ];
}	GEOMETRY, *PGEOMETRY;

typedef struct tObj{
		PGEOMETRY geo;
		int dirx,diry;
		int posx,posy;
		int jumpFrames;
		void (* draw) (struct tObj *);
		void (* clear) (struct tObj *);
		void (* move) (struct tObj *);
		void (* set_speed) (struct tObj *, int, int);
} OBJECT, *POBJECT;

// ---- Function labels ----

// Drivrutiner för grafisk display
void graphic_initialize(void);
void graphic_clear_screen(void);
void graphic_pixel_set(int x, int y);
void graphic_pixel_clear(int x, int y);

void init_app(void);

// Keyb
unsigned char keyb(void);
void activateRow(int row);
int readColumn(void);

// Ascii


void ascii_ctrl_bit_set (unsigned char x);
void ascii_ctrl_bit_clear (unsigned char x);
void ascii_write_cmd(unsigned char command);
void ascii_write_data(unsigned char data);
unsigned char ascii_read_status(void);
unsigned char ascii_read_data(void);
void ascii_write_controller(unsigned char byte);
unsigned char ascii_read_controller(void);
void ascii_init(void);
void app_init(void);
void ascii_write_string(char *str);
void intToString(int n, char *str);

// object functions
void draw_object(POBJECT o);
void clear_object(POBJECT o);
void move_object(POBJECT o);
void set_speed_object(POBJECT o, int dirx, int diry);

// Game logic functions
void handle_jump(POBJECT pplayer);
int check_collision(POBJECT p, POBJECT platform);

// Ball functions
void move_ballobject(POBJECT o);

// Tim6 functions
void timer6_interrupt();
void timer6_init();



void delay_250ns(void);
void delay_mikro(unsigned int us);
void delay_milli(unsigned int ms);

// ---- Global variables ----
static volatile int ticks;
static volatile int seconds;



// ---- GEOMETRY ----

GEOMETRY player_geometry = {
	49,
	11, 15,
	{
		{2,0}, {3,0}, {7,0}, {8,0},
		{1,1}, {4,1}, {6,1}, {9,1},
		{0,2}, {3,2}, {7,2}, {10,2},
		{0,3}, {2,3}, {8,3}, {10,3},
		{0,4}, {3,4}, {4,4}, {5,4}, {6,4}, {7,4}, {10,4},
		{1,5}, {9,5},
		{1,6}, {9,6},
		{1,7}, {3,7}, {7,7}, {9,7},
		{2,8}, {8,8},
		{3,9}, {4,9}, {5,9}, {6,9}, {7,9},
		{3,10}, {7,10},
		{3,11}, {7,11},
		{3,12}, {7,12}, 
		{3,13}, {5,13}, {7,13}, 
		{4,14}, {6,14}
	}
};

GEOMETRY platform_geometry = {
	40,
	20, 2,
	{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}, {17, 0}, {18, 0}, {19, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1}, {10, 1}, {11, 1}, {12, 1}, {13, 1}, {14, 1}, {15, 1}, {16, 1}, {17, 1}, {18, 1}, {19, 1}}

};

GEOMETRY ball_geometry =
{
	12, 	/* numpoints */
	4,4,	/* sizex,sizey */
	{
		/* px[0,1,2...]*/
		{0,1},{0,2},{1,0},{1,1},{1,2},{1,3},{2,0},{2,1},{2,2},{2,3},{3,1},{3,2}
	}
};

// ---- OBJECTS ----

OBJECT platform_low_left = {
	&platform_geometry,
	0,0,
	40,64-12,
	0,
	draw_object,
	clear_object,
	move_object,
	set_speed_object
};

OBJECT playerObject =  {
	&player_geometry,
	0,0,			// Dir x y
	0,64-15,	//64-15		// Pos x y
	0,				// Jump-frames
	draw_object,
	clear_object,
	move_object,
	set_speed_object
	
};


OBJECT ballobject =
{
	&ball_geometry, /* geometri för en boll */
	3,1,			/* inititala riktigtskoordinater */
	1,1,			/* initial startposition */
	0,
	draw_object,
	clear_object,
	move_ballobject,
	set_speed_object
};


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



// ---- DISPLAY ----

__attribute__((naked))
void graphic_initialize(void) {
	__asm volatile (" .HWORD 0xDFF0\n");
	__asm volatile (" BX LR\n");
}

__attribute__((naked))
void graphic_clear_screen(void) {
	__asm volatile (" .HWORD 0xDFF1\n");
	__asm volatile (" BX LR\n");
}

__attribute__((naked))
void graphic_pixel_set(int x, int y) {
	__asm volatile (" .HWORD 0xDFF2\n");
	__asm volatile (" BX LR\n");
}
__attribute__((naked))
void graphic_pixel_clear(int x, int y) {
	__asm volatile (" .HWORD 0xDFF3\n");
	__asm volatile (" BX LR\n");
}


// ---- Keyb ----
unsigned char keyb(void) {
    char key[] = {1,2,3,0xA,4,5,6,0xB,7,8,9,0xC,0xE,0,0xF,0xD};
    for (int i = 1; i < 5; i++) {
        activateRow(i);
        int column = readColumn();
        if (column != 0) {
            activateRow(0);
            return key[4*(i - 1) + (column - 1)];
        }
        
        }
		
            activateRow(0);
            return 0xFF;
    
}
void activateRow(int row) {
    switch (row) {
        case 1:
            *GPIO_D_ODR_HIGH = 0x10;
            break;
            
        case 2:
            *GPIO_D_ODR_HIGH = 0x20;
            break;
            
        case 3:
            *GPIO_D_ODR_HIGH = 0x40;
            break;
            
        case 4:
            *GPIO_D_ODR_HIGH = 0x80;
            break;
            
        default: 
            *GPIO_D_ODR_HIGH = 0;
    }
}

int readColumn(void) {
    char c = *GPIO_D_IDR_HIGH;
    if (c & 8) {
		return 4;
	}
	if (c & 4) {
		return 3;
	}
	if (c & 2) {
		return 2;
	}
	if (c & 1) {
		return 1;
	}
	return 0;


}


// ---- Object functions ----

void draw_object(POBJECT o){
	PGEOMETRY go = o->geo;
	int npoints = go->numPixels;
	for(int i = 0; i < npoints; i++){
		graphic_pixel_set(o->geo->pixels[i].x + o->posx, o->geo->pixels[i].y+ o->posy);
	}
}

void clear_object(POBJECT o){
	PGEOMETRY go = o->geo;
	int npoints = go->numPixels;
	for(int i = 0; i < npoints; i++){
		graphic_pixel_clear(o->geo->pixels[i].x + o->posx, o->geo->pixels[i].y+ o->posy);
	}	
}

void move_object(POBJECT o){
	o->clear(o);
	int newPosX = o->posx + o->dirx;
	int newPosY = o->posy + o->diry;
	
	if (newPosX < 0 || newPosX > 128- o->geo->sizex) {
		newPosX = o->posx;
	}
	if (newPosY < 0 || newPosY > 256) {
		newPosY = o->posy;
	}
	o->posx = newPosX;
	o->posy = newPosY;
	o->draw(o);
}

void set_speed_object(POBJECT o, int dirx, int diry) {
	o->dirx = dirx;
	o->diry = diry;
}

void set_jumpFrames_object(POBJECT o, int jumpFramess) {
	o->jumpFrames = jumpFramess;

}

void move_ballobject(POBJECT o){
	o->clear(o);
	int x = o->posx;
	int y = o->posy;
	int dx = o->dirx;
	int dy = o->diry;
	
	x+=dx;
	y+=dy;
	
	if(x < 1){
		dx =-dx; // Bounce if it hits left border
	}
	
	
	if(x + o->geo->sizex > 128){
		dx =-dx; // Bounce if it hits right border   - should no longer bounce, lose if right border
	}
	 
	if(y < 1){
		dy =-dy; // Bounce if it hits top border
	}
	if(y + o->geo->sizey > 64){
		dy =-dy; // Bounce if it hits bottom border
	
	}
	
	
	o->posx = x;
	o->posy = y;
	o->set_speed(o,dx,dy);
	o->draw(o);
}






// ---- Game logic ----

void handle_jump(POBJECT pplayer) {
		if (pplayer->jumpFrames != 0) {
			set_jumpFrames_object(pplayer, pplayer->jumpFrames - 1);	
		}
		if (pplayer->jumpFrames == 20) {
			pplayer->set_speed(pplayer,pplayer->dirx,2);
		}
		if (pplayer->jumpFrames == 0) {
			pplayer->set_speed(pplayer,pplayer->dirx,0);
		}
}

int check_collision(POBJECT ball, POBJECT paddle) {
	int collidesXaxis = (ball->posx >= paddle->posx) && (ball->posx < paddle->posx + paddle->geo->sizex);
	int collidesYaxis = (ball->posy >= paddle->posy) && (ball->posy < paddle->posy + paddle->geo->sizey);
	return collidesXaxis && collidesYaxis;
}

// ---- Tim6 ----

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

// ---- Ascii ----
void ascii_ctrl_bit_set (unsigned char x) {
	*GPIO_E_ODR_LOW |= x;
	*GPIO_E_ODR_LOW |= B_SELECT;
}
void ascii_ctrl_bit_clear (unsigned char x) {
	*GPIO_E_ODR_LOW &= ~x;
	*GPIO_E_ODR_LOW |= B_SELECT;
	
}

void ascii_write_cmd(unsigned char command) {
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	
	ascii_write_controller(command);
	
}
void ascii_write_data(unsigned char data) {
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	
	ascii_write_controller(data);
	
}
	
void ascii_write_controller(unsigned char byte) {
	//delay_delay250ns
	ascii_ctrl_bit_set(B_E);
	*GPIO_E_ODR_HIGH = byte;
	delay_250ns();
	ascii_ctrl_bit_clear(B_E);
	
	
}

unsigned char ascii_read_controller(void) {
	*GPIO_E_MODER = 0x00005555;
	ascii_ctrl_bit_set(B_E);
	//delay_250ns();
	//delay_250ns();
	unsigned char rw = *GPIO_E_IDR_HIGH;
	ascii_ctrl_bit_clear(B_E);
	*GPIO_E_MODER = 0x55555555;
	return rw;
	
	
}

unsigned char ascii_read_status(void) {
	*GPIO_E_MODER = 0x00005555;
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_set(B_RW);
	unsigned char rw = ascii_read_controller();
	*GPIO_E_MODER = 0x55555555;
	return rw;
}

unsigned char ascii_read_data(void) {
	*GPIO_E_MODER &= 0x0000FFFF;
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_set(B_RW);
	unsigned char rw = ascii_read_controller();
	*GPIO_E_MODER |= 0x55550000;
	return rw;
}


void ascii_init(void) {
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	
	ascii_write_cmd(0b00111000); // 2 rader, 5 x 8.
	
	ascii_write_cmd(0b00001110); // Display på, markör på, blinkande markör av.
	
	ascii_write_cmd(0b00000110); // Increment, skift av.
	
}



void ascii_gotoxy(int row, int col) {
	int address = row -1;
	if (col == 2) {
		address = address + 0x40;
	}
	ascii_write_cmd(0x80 | address);
}

void ascii_write_char(unsigned char c) {
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_set(B_RW);
	while ((ascii_read_status() & 0x80) == 0x80) {
		
	}
	delay_mikro(4);
	ascii_write_data(c);
	delay_mikro(1);
	
}

void ascii_clear_display( void ) {
	while ((ascii_read_status() & 0x80) == 0x80) {
		
	}
	delay_mikro(4);
	ascii_write_cmd(1);
	delay_mikro(1);
}

void intToString(int n, char *str) {
	int temp = n;
	int digits = 0;
	while (temp) {
		temp /= 10;
		digits++;
	}
	
	for (int i = digits -1; i >= 0; i--) {
		str[i] = '0' + (n % 10);
		n /= 10;
	}
	str[digits] = '\0';
	
}

void ascii_write_string(char *str) {
	while (*str) {
		ascii_write_char(*str++);
	}
}


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







	

	


