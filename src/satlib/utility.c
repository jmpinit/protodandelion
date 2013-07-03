#include <stdio.h>
#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "sat.h"
#include "sat_util.h"

#include "satlib/utility.h"

// TODO special sat_error function prints name of current function

// (filename)
int satlib_file_exists(lua_State *L) {
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
