#include "objects.h"
	



void move_ballobject(POBJECT o);
void set_jumpFrames_object(POBJECT o, int jumpFramess);
void set_speed_object(POBJECT o, int dirx, int diry);
void move_object(POBJECT o);
void clear_object(POBJECT o);
void draw_object(POBJECT o);
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
