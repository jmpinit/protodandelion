#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL/SDL.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "sat.h"
#include "sat_util.h"
#include "list.h"

#include "satlib/initialize.h"

// (name)
int satlib_part_type_new(lua_State *L) {
	if(lua_type(L, 1) == LUA_TSTRING) {
		// == get args ==
		const char* name = lua_tostring(L, 1);

		// == create part ==

		SatPartInfo* part = calloc(1, sizeof(SatPartInfo));

		// save the name
		part->name = calloc(strlen(name), sizeof(char));
		strcpy(part->name, name);

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
int satlib_part_type_add_sprite(lua_State *L) {
	if(lua_type(L, 1) == LUA_TSTRING && lua_type(L, 2) == LUA_TSTRING && lua_type(L, 3) == LUA_TNUMBER) {
		// get args
		const char* name = lua_tostring(L, 1);
		const char* filename = lua_tostring(L, 2);
		Rot rot = (Rot)lua_tonumber(L, 3);

		// fail if the rotation is out of bounds
		if(rot < 0 || rot >= 4)
			return luaL_error(L, "%s: rotation of %d is out of bounds.", __func__, rot);

		// get the part info or return failure
		SatPartInfo* partinfo = info_by_name((char*)name);
		if(partinfo == NULL)
			return luaL_error(L, "%s: no part named %s.", __func__, name);

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

		printf("added sprite to %s at %d degrees\n", name, rot*90);
	} else {
		return luaL_error(L, "%s: argument should be name, filename, rotation index.", __func__);
	}

	return 0;
}

// (name, index, x, y, rot)
// WARN: assumes empty connectors already set up
int satlib_part_type_set_connector(lua_State *L) {
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
		SatPartInfo* partinfo = info_by_name((char*)name);
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
int satlib_part_type_init_connectors(lua_State *L) {
	if(	lua_type(L, 1) == LUA_TSTRING &&
		lua_type(L, 2) == LUA_TNUMBER
	) {
		// get args
		const char* name = lua_tostring(L, 1);
		unsigned char num = (unsigned char)lua_tonumber(L, 2);

		// get part and return failure otherwise
		SatPartInfo* partinfo = info_by_name((char*)name);
		if(partinfo == NULL)
			return luaL_error(L, "%s: no part named %s.", __func__, name);

		// create connectors
		partinfo->connectors = calloc(num, sizeof(Connector*));
		for(int i=0; i<num; i++)
			partinfo->connectors[i] = calloc(1, sizeof(Connector));
		partinfo->num_connectors = num;
	} else {
		return luaL_error(L, "%s: argument should be part name, number of connectors.", __func__);
	}

	return 0;
}
