#!/usr/bin/lua

-- TODO set the connectors
-- TODO wrap repeated code in nice utility functions

satlib.load_sprite("res/empty.bmp", 0, satlib.d0)
satlib.load_sprite("res/empty.bmp", 0, satlib.d90)
satlib.load_sprite("res/empty.bmp", 0, satlib.d180)
satlib.load_sprite("res/empty.bmp", 0, satlib.d270)
print("loaded empty sprite.")

satlib.load_sprite("res/base.bmp", 1, satlib.d0)
satlib.load_sprite("res/base.bmp", 1, satlib.d90)
satlib.load_sprite("res/base.bmp", 1, satlib.d180)
satlib.load_sprite("res/base.bmp", 1, satlib.d270)
print("loaded base sprite.")

--[[
		temp->connectors[0]->direction = LEFT
		temp->connectors[1]->direction = UP
		temp->connectors[2]->direction = RIGHT
		temp->connectors[3]->direction = DOWN
--]]

satlib.load_sprite("res/mainframe.bmp", 2, satlib.d0)
satlib.load_sprite("res/mainframe.bmp", 2, satlib.d90)
satlib.load_sprite("res/mainframe.bmp", 2, satlib.d180)
satlib.load_sprite("res/mainframe.bmp", 2, satlib.d270)
print("loaded mainframe sprite.")

--[[
		temp->connectors[0]->direction = LEFT
		temp->connectors[1]->direction = UP
		temp->connectors[2]->direction = RIGHT
		temp->connectors[3]->direction = DOWN
--]]

satlib.load_sprite("res/solar_0.bmp",	3, satlib.d0)
satlib.load_sprite("res/solar_90.bmp",	3, satlib.d90)
satlib.load_sprite("res/solar_180.bmp",	3, satlib.d180)
satlib.load_sprite("res/solar_270.bmp",	3, satlib.d270)
print("loaded solar sprite.")

--		temp->connectors[0]->direction = LEFT

satlib.load_sprite("res/dish_wide_0.bmp",	4, satlib.d0)
satlib.load_sprite("res/dish_wide_90.bmp",	4, satlib.d90)
satlib.load_sprite("res/dish_wide_180.bmp",	4, satlib.d180)
satlib.load_sprite("res/dish_wide_270.bmp",	4, satlib.d270)
print("loaded dish_wide sprite.")

--[[
		temp->connectors[0]->position.x = 0
		temp->connectors[0]->position.y = 1
		temp->connectors[0]->direction = LEFT
--]]
