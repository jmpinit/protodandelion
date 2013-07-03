#ifndef CONSTRUCT_H
#define CONSTRUCT_H

int satlib_sat_new(lua_State *L);
int satlib_sat_select(lua_State *L); 
int satlib_sat_part_last(lua_State *L);
int satlib_sat_part_go(lua_State *L);
int satlib_sat_part_add(lua_State *L);

#endif
