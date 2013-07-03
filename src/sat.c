#include "SDL/SDL.h"

#include "sat.h"
#include "sat_util.h"
#include "list.h"

struct Node* sat_partinfos;

#include "sat_lua.c"

void sat_render(struct Satellite* sat, SDL_Surface* canvas) {
}

void sat_init(lua_State* L) {
	sat_partinfos = calloc(1, sizeof(struct Node));

	// load the library
	openlualibs(L);

	// run the load script
	const char* startup = "src/scripts/load.lua";
	int s = luaL_loadfile(L, startup);

	if ( s==0 ) {
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
		if(s) fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
		printf("Called script.");
	} else {
		fprintf(stderr, "Lua load error: %s\n", lua_tostring(L, -1));
		exit(0);
	}
}
