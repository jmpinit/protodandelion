#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SDL/SDL.h"
//#include "SDL/SDL_thread.h"

/* Include the Lua API header files. */
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "satellite.h"

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


static void openlualibs(lua_State *l)
{
	static const luaL_reg lualibs[] =
	{
		{ "base",       luaopen_base },
		{ NULL,         NULL }
	};

	const luaL_reg *lib;

	for (lib = lualibs; lib->func != NULL; lib++)
	{
		lib->func(l);
		lua_settop(l, 0);
	}
}

int lua_test_callback(lua_State* lua)
{
	if (lua_gettop(lua) == 1 && // make sure exactly one argument is passed
			lua_isfunction(lua, -1)) // and that argument (which is on top of the stack) is a function
	{
		lua_pushnumber(lua, 3); // push first argument to the function
		lua_pcall(lua, 1, 0, 0); // call a function with one argument and no return values
	}
	return 0; // no values are returned from this function
}

int main() {
	lua_State *l;
	l = lua_open();
	openlualibs(l);

	int s = luaL_loadfile(l, "src/scripts/test.lua");
	if ( s==0 ) {
		s = lua_pcall(l, 0, LUA_MULTRET, 0);
		printf("Called script.");
	} else {
		printf("Couldn't load script");
	}

	/* Remember to destroy the Lua State */
	lua_close(l);

	SDL_init();
	sat_init();

	SDL_Rect src, dest;
	 
	src.x = 0;
	src.y = 0;
	src.w = sat_sprites[0]->w;
	src.h = sat_sprites[0]->h;
	 
	dest.x = 100;
	dest.y = 100;
	dest.w = sat_sprites[0]->w;
	dest.h = sat_sprites[0]->h;
	 
	SDL_BlitSurface(sat_sprites[0], &src, screen, &dest);
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
							dest.x++;
							SDL_BlitSurface(sat_sprites[0], &src, screen, &dest);
							SDL_Flip(screen);
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
