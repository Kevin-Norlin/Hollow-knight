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

	#define MAX_POINTS	50

// Drivrutiner för grafisk display
void graphic_initialize(void);
void graphic_clear_screen(void);
void graphic_pixel_set(int x, int y);
void graphic_pixel_clear(int x, int y);

void init_app(void);



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
		void (* draw) (struct tObj *);
		void (* clear) (struct tObj *);
		void (* move) (struct tObj *);
		void (* set_speed) (struct tObj *, int, int);
} OBJECT, *POBJECT;

// object functions
void draw_object(POBJECT o);
void clear_object(POBJECT o);
void move_object(POBJECT o);

void set_speed_object(POBJECT o, int dirx, int diry);



// Player geo

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

OBJECT player =  {
	&player_geometry,
	0,0,
	0,0,
	
};



void main(void)
{
	app_init();
	draw_object(&player);
}

void app_init(void) {
	// port D medium speed
	*((volatile unsigned int *)0x40020C08) = 0x55555555;
	/* starta klockor port D och E */
	* ( (unsigned long *) 0x40023830) = 0x18;
	
	// 2 pin output, 2 pin input, 4 pin output
    *GPIO_D_MODER = 0x55000000;
	// Open drain
    *GPIO_D_OTYPER = 0xFF;
	// 2 pin Floating - 2 pin pull down - 4 pin reserved 
    *GPIO_D_PUPDR = 0xAAFFFF;
}



// DISPLAY

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
	if (newPosX < 0 || newPosX > 128) {
		newPosX = o->posx;
	}
	if (newPosY < 0 || newPosY > 256) {
		newPosY = o->posy;
	}
	o->posx = newPosX;
	o->posy = newPosY;
	o->draw(o);
	// Kanske också set_speed?
}

void set_speed_object(POBJECT o, int dirx, int diry) {
	o->dirx = dirx;
	o->diry = diry;
}
	
	


