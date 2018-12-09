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
#include "beta.h"
#include "term.h"
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

int main() {
	lstate = luaL_newstate();
  luaL_openlibs(lstate);

  /*
	// do beta setup FIXME
	Beta* beta = beta_create(1024);
	beta_load(beta, "test.bin");

	// 280 cycle test using lab6.uasm
	for(int i=0; i < 300; i++) {
		if(i == 264)
			beta_interrupt(beta);
		else
			beta_tick(beta);

		if(beta->pc == 0xC) {
			printf("failed with error code %d.\n", beta->registers[0]);
			exit(1);
		}
	}

	beta_dump_info(beta);
	printf("========\n");
	beta_dump_registers(beta);
	printf("========\n");
  */

	// create a terminal
	Terminal* terminal = term_init(640/11, 480/13, "res/font.png");
	term_puts(terminal, "hello world!");

	SDL_init();
	sat_init(lstate);

  /*
	while(true) {
		term_render(terminal, screen);
		SDL_Flip(screen);

		SDL_Event event;

		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;
			}
		}

		SDL_Delay(32);
	}
  */

	while(true) {
		Node* rootsatnode = satellites;
		Satellite* rootsat = (Satellite*)rootsatnode->data;

		if(rootsat != NULL) {
			//sat_render(rootsat, screen);

			SDL_Flip(screen);
		}

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
						case SDLK_ESCAPE: exit(0);
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
