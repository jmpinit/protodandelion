#include "SDL/SDL.h"

#include "sat.h"

struct SatPartInfo* sat_parts[SAT_N_PARTS];
struct Satellite* satellites[4];

#include "sat_lua.c"

void sat_render(struct Satellite* sat, SDL_Surface* canvas) {
	struct SatPartNode* last = sat->first_part;
	while(last->next != NULL) {
		last = last->next;
	}
}

void sat_init(lua_State* L) {
	// load the library
	luaopen_satlib(L);

	// run the load script
	const char* startup = "src/scripts/load.lua";
	int s = luaL_loadfile(L, startup);

	if ( s==0 ) {
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
		if(s) fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
		printf("Called script.");
	} else {
		printf("Couldn't load sprite load script (%s).", startup);
		exit(0);
	}
}
