#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static int load_sprite(lua_State *L);

static const luaL_reg satlib[] =
{
	{ "load_sprite",	load_sprite },
	{ NULL,				NULL }
};

int luaopen_satlib(lua_State *L) {
	luaL_openlib(L, "satlib", satlib, 0);

	// push rotations
	lua_pushnumber(L, D0);
	lua_setfield(L, -2, "d0");
	lua_pushnumber(L, D90);
	lua_setfield(L, -2, "d90");
	lua_pushnumber(L, D180);
	lua_setfield(L, -2, "d180");
	lua_pushnumber(L, D270);
	lua_setfield(L, -2, "d270");

	return 1;
}

// (filename, part id, rotation index
static int load_sprite (lua_State *L) {
	int success;

	if(lua_type(L, 1) == LUA_TSTRING && lua_type(L, 2) == LUA_TNUMBER && lua_type(L, 3) == LUA_TNUMBER) {
		// get args
		const char * file = lua_tostring(L, 1);
		unsigned int partID = (unsigned int)lua_tonumber(L, 2);
		Rot rot = (Rot)lua_tonumber(L, 3);

		// load sprite
		struct SatPartInfo* part = sat_parts[partID];
		if(part == NULL) {
			part = calloc(1, sizeof(struct SatPartInfo));
			sat_parts[partID] = part;
			part->sprites = calloc(4, sizeof(SDL_Surface*));
		}

		SDL_Surface* temp = SDL_LoadBMP(file);
		if (temp == NULL) {
			printf("Unable to load bitmap: %s\n", SDL_GetError());
			exit(0);
		}

		part->sprites[rot] = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);

		// the unrotated sprite determines the width and height
		if(rot == D0) {
			part->w = part->sprites[rot]->w / SAT_TILE_WIDTH;
			part->h = part->sprites[rot]->h / SAT_TILE_HEIGHT;
		}

		success = 1;
	} else {
		success = 0;
	}

	lua_pushnumber(L, success);
	return 1;
}

struct SatPartInfo* new_part(SatPartID type, unsigned int numConnectors) {
	struct SatPartInfo* part = calloc(1, sizeof(struct SatPart));

	part->sprites = (SDL_Surface**)calloc(4, sizeof(SDL_Surface*));

	part->connectors = (struct Connector**)calloc(numConnectors, sizeof(struct Connector*));
	for(int i=0; i < numConnectors; i++) 
		part->connectors[i] = (struct Connector*)calloc(1, sizeof(struct Connector));
	part->num_connectors = numConnectors;

	sat_parts[type] = part;

	return part;
}
