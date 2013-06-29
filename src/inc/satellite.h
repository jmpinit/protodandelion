#ifndef SAT_H
#define SAT_H

SDL_Surface* sat_sprites[4];

enum {
	base,
	mainframe,
	solar,
	wide_dish
} SatPart;

void sat_init(void);

#endif
