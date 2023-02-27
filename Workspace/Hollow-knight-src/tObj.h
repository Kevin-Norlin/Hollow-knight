#ifndef TOBJ_H
#define TOBJ_H


typedef struct {
	char x,y;
} PIXEL,*PPIXEL;


typedef struct {
	int numPixels;
	int sizex, sizey;
	PIXEL pixels[ MAX_POINTS ];
}	GEOMETRY, *PGEOMETRY;

typedef struct tObj{
	PGEOMETRY	geo;
	int dirx, diry;
	int posx, posy;
	void (* draw) (struct tObj *, int, int);
	void (* clear) (struct tObj *);
	void (* move) (struct tObj *);
	void (* set_speed) (struct tObj *, int, int);
}

#endif