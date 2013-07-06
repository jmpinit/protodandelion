#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SDL/SDL.h"
//#include "SDL/SDL_thread.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "sat.h"
#include "list.h"

lua_State* lstate;
SDL_Surface* screen;

void cleanup() {
	lua_close(lstate);
	SDL_Quit();
}

void SDL_init() {
	setbuf(stdout, NULL);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	screen = SDL_SetVideoMode(640, 480, 0, SDL_HWPALETTE);
	
	if (screen == NULL) {
		printf("Couldn't set screen mode to 640 x 480: %s\n", SDL_GetError());
		exit(1);
	}
	
	SDL_WM_SetCaption("dandelion", NULL);
	
	atexit(cleanup);
}

/*void draw_stuff(int x, int y, Satellite* sat) {
	SDL_Rect src, dest;
	 
	src.x = 0;
	src.y = 0;

	for(int dy = 0; dy < SAT_HEIGHT; dy++) {
		for(int dx = 0; dx < SAT_WIDTH; dx++) {
			int index = sat->parts[dy*SAT_WIDTH+dx];
			printf("%d\n", index);
			SDL_Surface* sprite = sat_parts[index]->sprite;
			src.w = sprite->w;
			src.h = sprite->h;
			 
			dest.x = x + dx*8;
			dest.y = y + dy*8;
			dest.w = src.w;
			dest.h = src.h;
			 
			SDL_BlitSurface(sprite, &src, screen, &dest);
		}
	}
}*/

int t;
int mode = 0;

int main() {
	lstate = lua_open();

	SDL_init();
	sat_init(lstate);

	t = 0;

	while(true) {
		Node* rootsatnode = satellites;
		Satellite* rootsat = (Satellite*)rootsatnode->data;

		for(int i=0; i < screen->w * screen->h * 4; i++) {
			switch(mode) {
				case 0: ((char*)screen->pixels)[i] = t*(((t>>12)|(t>>8))&(63&(t>>4))); break;
				case 1: ((char*)screen->pixels)[i] = t*(42&t>>10); break;
				case 2: ((char*)screen->pixels)[i] = t*2*(21&t>>11); break;
				case 3: ((char*)screen->pixels)[i] = t*((42&t>>10)%14); break;
				case 4: ((char*)screen->pixels)[i] = t*(5+((t>>11)&5)); break;
				case 5: ((char*)screen->pixels)[i] = ((t*("36364689"[t>>13&7]&15))/12&128) + (((((t>>12)^(t>>12)-2)%11*t)/4|t>>13)&127); break;
				case 6: ((char*)screen->pixels)[i] = ((t>>12)^(t>>12)-2)%11; break;
				case 7: ((char*)screen->pixels)[i] = t*(t>>9|t>>13)&16; break;
				case 8: ((char*)screen->pixels)[i] = t*5&(t>>7)|t*3&(t*4>>10); break;
				case 9: ((char*)screen->pixels)[i] = (t*5&t>>7)|(t*3&t>>10); break;
				case 10: ((char*)screen->pixels)[i] = t*9&t>>4|t*5&t>>7|t*3&t/1024; break;
				case 11: ((char*)screen->pixels)[i] = t*(0xCA98>>(t>>9&14)&15)|t>>8; break;
				case 12: ((char*)screen->pixels)[i] = (t>>8^t>>10|t>>14|t)&63; break;
				case 13: ((char*)screen->pixels)[i] = (t*9&t>>4|t*5&t>>7|t*3&t/1024)-1; break;
				case 14: ((char*)screen->pixels)[i] = t>>4|t&((t>>5)/(t>>7-(t>>15)&-t>>7-(t>>15))); break;
				case 15: ((char*)screen->pixels)[i] = t>>6&1?t>>5:-t>>4; break;
				case 16: ((char*)screen->pixels)[i] = t>>6^t>>8|t>>12|t&63; break;
			}
			t++;
		}

		if(rootsat != NULL) {
			sat_render(rootsat, screen);

			SDL_Flip(screen);
		}
		t++;

		SDL_Event event;

		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_RIGHT:
							mode++;
							mode = mode%17;
							break;
						case SDLK_UP:
							break;
						case SDLK_LEFT:
							mode--;
							mode = mode%17;
							break;
						case SDLK_DOWN:
							break;
						case SDLK_ESCAPE:
							exit(0);
							break;

						default:
							break;
					}
					break;
			}
		}

		SDL_Delay(32);
	}

	return 0;
}
