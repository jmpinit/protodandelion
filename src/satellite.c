#include "SDL/SDL.h"

#include "satellite.h"

SDL_Surface* sat_sprites[4];

void load_sprite(const char* file, unsigned int index) {
	SDL_Surface *temp = SDL_LoadBMP(file);
	if (temp == NULL) {
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		exit(0);
	}
	sat_sprites[index] = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
}

void load_sprites() {
	load_sprite("res/base.bmp"		, 0);
	load_sprite("res/mainframe.bmp"	, 1);
	load_sprite("res/solar.bmp"		, 2);
	load_sprite("res/wide_dish.bmp"	, 3);
}

void sat_init() {
	load_sprites();
}
