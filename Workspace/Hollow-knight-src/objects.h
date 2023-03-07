
#ifndef	OBJECTS_H
#define	OBJECTS_H

	#define MAX_POINTS	50
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

extern GEOMETRY player_geometry;
extern GEOMETRY platform_geometry;
extern GEOMETRY ball_geometry;
extern OBJECT platform_low_left;
extern OBJECT playerObject;
extern OBJECT ballobject;

extern void move_ballobject(POBJECT o);
extern void set_jumpFrames_object(POBJECT o, int jumpFramess);
extern void set_speed_object(POBJECT o, int dirx, int diry);
extern void move_object(POBJECT o);
extern void clear_object(POBJECT o);
extern void draw_object(POBJECT o);



#endif