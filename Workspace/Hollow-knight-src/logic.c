#include "objects.h"
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