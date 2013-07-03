// TODO special sat_error function prints name of current function
#include <stdio.h>
#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define FAILURE 0
#define SUCCESS 1

static int part_new(lua_State *L);
static int part_add_sprite(lua_State *L);
static int part_set_connector(lua_State *L);
static int part_init_connectors(lua_State *L);
static int file_exists(lua_State *L);

static const luaL_reg satlib[] =
{
	{ "part_new",				part_new },
	{ "part_add_sprite",		part_add_sprite },
	{ "part_init_connectors",	part_init_connectors },
	{ "part_set_connector",		part_set_connector },
	{ "file_exists",			file_exists },
	{ NULL,						NULL }
};

int luaopen_satlib(lua_State *L) {
	luaL_openlib(L, "satlib", satlib, 0);
	return 1;
}

static void openlualibs(lua_State *l) {
	static const luaL_reg lualibs[] =
	{
		{ "base",       luaopen_base },
		{ "satlib",		luaopen_satlib },
		{ NULL,         NULL }
	};

	const luaL_reg *lib;

	for (lib = lualibs; lib->func != NULL; lib++) {
		lib->func(l);
		lua_settop(l, 0);
	}
}

// LIBRARY

// (name)
static int part_new(lua_State *L) {
	if(lua_type(L, 1) == LUA_TSTRING) {
		// == get args ==
		const char* tempName = lua_tostring(L, 1);

		// == create part ==

		struct SatPartInfo* part = calloc(1, sizeof(struct SatPartInfo));

		// save the name
		char* name = calloc(strlen(tempName), sizeof(char));
		strcpy(name, tempName);
		part->name = name;

		// setup sprite array
		part->sprites = calloc(4, sizeof(SDL_Surface*));

		// add the new part to the list 
		list_add(sat_partinfos, part);
	} else {
		return luaL_error(L, "%s: argument should be part name.", __func__);
	}

	return 0;
}

// (name, filename, rotation index
static int part_add_sprite(lua_State *L) {
	if(lua_type(L, 1) == LUA_TSTRING && lua_type(L, 2) == LUA_TSTRING && lua_type(L, 3) == LUA_TNUMBER) {
		// get args
		const char* name = lua_tostring(L, 1);
		const char* filename = lua_tostring(L, 2);
		Rot rot = (Rot)lua_tonumber(L, 3);

		// get the part info or return failure
		struct SatPartInfo* partinfo = info_by_name((char*)name);
		if(partinfo == NULL) {
			return luaL_error(L, "%s: no part named %s.", __func__, name);
		}

		// load the image
		SDL_Surface* temp = SDL_LoadBMP(filename);
		if (temp == NULL) {
			printf("Unable to load bitmap: %s\n", SDL_GetError());
			return luaL_error(L, "%s: unable to load bitmap %s.", __func__, filename);
		}

		partinfo->sprites[rot] = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);

		// the unrotated sprite determines the width and height
		if(rot == D0) {
			partinfo->w = partinfo->sprites[rot]->w / SAT_TILE_WIDTH;
			partinfo->h = partinfo->sprites[rot]->h / SAT_TILE_HEIGHT;
		}
	} else {
		return luaL_error(L, "%s: argument should be name, filename, rotation index.", __func__);
	}

	return 0;
}

// (name, index, x, y, rot)
// WARN: assumes empty connectors already set up
static int part_set_connector(lua_State *L) {
	if(	lua_type(L, 1) == LUA_TSTRING &&
		lua_type(L, 2) == LUA_TNUMBER &&
  		lua_type(L, 3) == LUA_TNUMBER &&
		lua_type(L, 4) == LUA_TNUMBER	 
	) {
		// get args
		const char* name = lua_tostring(L, 1);
		int i = (int)lua_tonumber(L, 2);
		int x = (int)lua_tonumber(L, 3);
		int y = (int)lua_tonumber(L, 4);
		int dir = (int)lua_tonumber(L, 5);

		// make sure part info exists
		struct SatPartInfo* partinfo = info_by_name((char*)name);
		if(partinfo == NULL)
			return luaL_error(L, "%s: no part named %s.", __func__, name);

		if(i < partinfo->num_connectors) {
			// set the connector 
			partinfo->connectors[i]->position.x = x;
			partinfo->connectors[i]->position.y = y;
			partinfo->connectors[i]->direction = dir;
		} else {
			return luaL_error(L, "%s: no connector index of %d.", __func__, i);
		}
	} else {
		return luaL_error(L, "%s: arguments should be name, index, x, y, rot.", __func__);
	}

	return 0;
}

// (name, num)
// WARN: assumes empty connector already set up
// LIMIT: 256 connector
static int part_init_connectors(lua_State *L) {
	if(	lua_type(L, 1) == LUA_TSTRING &&
		lua_type(L, 2) == LUA_TNUMBER
	) {
		// get args
		const char* name = lua_tostring(L, 1);
		unsigned char num = (unsigned char)lua_tonumber(L, 2);

		// get part and return failure otherwise
		struct SatPartInfo* partinfo = info_by_name((char*)name);
		if(partinfo == NULL)
			return luaL_error(L, "%s: no part named %s.", __func__, name);

		// create connectors
		partinfo->connectors = calloc(num, sizeof(struct Connector*));
		for(int i=0; i<num; i++)
			partinfo->connectors[i] = calloc(1, sizeof(struct Connector));
		partinfo->num_connectors = num;
	} else {
		return luaL_error(L, "%s: argument should be part name, number of connectors.", __func__);
	}

	return 0;
}

// (filename)
static int file_exists(lua_State *L) {
	if(	lua_type(L, 1) == LUA_TSTRING) {
		// get args
		const char* filename = lua_tostring(L, 1);

		FILE* file = fopen(filename, "r");
		if(file != NULL) {
			fclose(file);
			lua_pushboolean(L, true);
			return 1;
		}

		lua_pushnumber(L, false);
		return 1;
	} else {
		return luaL_error(L, "%s: argument should be filename.", __func__);
	}
}

/* HOW TO LOAD AN IMAGE FILE
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
*/
