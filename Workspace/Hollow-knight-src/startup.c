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

// Keyb
unsigned char keyb(void);
void activateRow(int row);
int readColumn(void);





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

// object functions
void draw_object(POBJECT o);
void clear_object(POBJECT o);
void move_object(POBJECT o);
void set_speed_object(POBJECT o, int dirx, int diry);

// Game logic functions
void handle_jump(POBJECT pplayer);


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

GEOMETRY platform_geometry = {
	40,
	20, 2,
	{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}, {17, 0}, {18, 0}, {19, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1}, {10, 1}, {11, 1}, {12, 1}, {13, 1}, {14, 1}, {15, 1}, {16, 1}, {17, 1}, {18, 1}, {19, 1}}

};

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
	0,64-15,			// Pos x y
	0,				// Jump-frames
	draw_object,
	clear_object,
	move_object,
	set_speed_object
	
};





void main(void)
{
	graphic_initialize();
	graphic_clear_screen();
	app_init();
	char c;
	OBJECT player = playerObject;
	POBJECT pplayer = &player;
	OBJECT platform_left = platform_low_left;
	POBJECT pplatform_low_left = &platform_left;
	draw_object(pplatform_low_left);
	draw_object(pplayer);
	
	while (1) {
		pplayer->move(pplayer);
		handle_jump(pplayer);
		int speed = pplayer->diry;
		c = keyb();
		switch(c) {
			case 4: pplayer->set_speed(pplayer, -2, 0); break;
			case 6: pplayer->set_speed(pplayer,2,0); break;
			case 2: if (pplayer->jumpFrames == 0) { pplayer->set_speed(pplayer,pplayer->dirx,-2); pplayer->jumpFrames=20; break; }
			default: pplayer->set_speed(pplayer,0,pplayer->diry); break;
		}
	}
	
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
	// Kanske också set_speed?
}

void set_speed_object(POBJECT o, int dirx, int diry) {
	o->dirx = dirx;
	o->diry = diry;
}

void set_jumpFrames_object(POBJECT o, int jumpFramess) {
	int n = jumpFramess;
	o->jumpFrames = jumpFramess;
	int burh = o->jumpFrames;
}



// Game logic

void handle_jump(POBJECT pplayer) {
		if (pplayer->jumpFrames != 0) {
			set_jumpFrames_object(pplayer, pplayer->jumpFrames - 1);	
		}
		if (pplayer->jumpFrames == 10) {
			pplayer->set_speed(pplayer,pplayer->dirx,2);
		}
		if (pplayer->jumpFrames == 0) {
			pplayer->set_speed(pplayer,pplayer->dirx,0);
		}
}
	
	


