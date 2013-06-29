#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SDL/SDL.h"
//#include "SDL/SDL_thread.h"

SDL_Surface *screen;

void cleanup() {
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

void load_sprites() {
	SDL_Surface *image;
	SDL_Surface *temp;

	temp = SDL_LoadBMP("res/robot.bmp");
	if (temp == NULL) {
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		exit(0);
	}

	image = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	SDL_Rect src, dest;
	 
	src.x = 0;
	src.y = 0;
	src.w = image->w;
	src.h = image->h;
	 
	dest.x = 100;
	dest.y = 100;
	dest.w = image->w;
	dest.h = image->h;
	 
	SDL_BlitSurface(image, &src, screen, &dest);

	SDL_Flip(screen);

	SDL_FreeSurface(image);
}

int main() {
	SDL_init();
	load_sprites();

	while(true) {
		SDL_Event event;

		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
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
