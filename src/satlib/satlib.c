#include <stdio.h>
#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// LIBRARY SOURCE

#include "satlib/satlib.h"

#include "satlib/initialize.h"
#include "satlib/construct.h"
#include "satlib/utility.h"

// LIBRARY INITIALIZATION

static const luaL_reg satlib[] = {
	// initilializers
	{ "part_type_new",				satlib_part_type_new },
	{ "part_type_add_sprite",		satlib_part_type_add_sprite },
	{ "part_type_init_connectors",	satlib_part_type_init_connectors },
	{ "part_type_set_connector",	satlib_part_type_set_connector },

	// satellite construction
	{ "sat_new",					satlib_sat_new},
	{ "sat_select",					satlib_sat_select},
	{ "sat_part_last",				satlib_sat_part_last},
	{ "sat_part_go",				satlib_sat_part_go},
	{ "sat_part_add",				satlib_sat_part_add},

	// utilities
	{ "file_exists",				satlib_file_exists },
	{ NULL,							NULL }
};

int luaopen_satlib(lua_State *L) {
	luaL_openlib(L, "satlib", satlib, 0);
	return 1;
}

void openlualibs(lua_State *l) {
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
