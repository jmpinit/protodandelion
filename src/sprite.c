#include "sat.h"
#include "SDL/SDL.h"

void sprite_render(SDL_Surface* canvas, struct SatPart* part, int x, int y, Rot rot) {
	SDL_Surface* sprite = part->info->sprites[rot];

	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = sprite->w;
	src.h = sprite->h;

	Pt corr;
	switch(rot) {
		case D0:
			corr.x = 0;
			corr.y = 0;
			break;
		case D90:
			corr.x = -(part->info->h-1);
			corr.y = 0;
			break;
		case D180:
			corr.x = -(part->info->w-1);
			corr.y = -(part->info->h-1);
			break;
		case D270:
			corr.x = 0;
			corr.y = -(part->info->w-1);
			break;
	}

	dest.x = (x + corr.x) * SAT_TILE_WIDTH;
	dest.y = (y + corr.y) * SAT_TILE_HEIGHT;
	dest.w = src.w;
	dest.h = src.h;

	SDL_BlitSurface(sprite, &src, canvas, &dest);
}

