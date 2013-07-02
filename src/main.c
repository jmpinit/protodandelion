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

static void openlualibs(lua_State *l)
{
	static const luaL_reg lualibs[] =
	{
		{ "base",       luaopen_base },
		{ NULL,         NULL }
	};

	const luaL_reg *lib;

	for (lib = lualibs; lib->func != NULL; lib++) {
		lib->func(l);
		lua_settop(l, 0);
	}
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

int main() {
	lstate = lua_open();
	openlualibs(lstate);

	SDL_init();
	sat_init(lstate);

	//sat_render(satellites[0], screen);

	SDL_Flip(screen);

	while(true) {
		SDL_Event event;

		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_RIGHT:
							break;
						case SDLK_UP:
							break;
						case SDLK_LEFT:
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

		SDL_Delay(16);
	}

	return 0;
}
