#!/usr/bin/lua

-- require 'constants'

LEFT	= 0
TOP		= 1
RIGHT	= 2
DOWN	= 3

D0		= 0
D90		= 1
D180	= 2
D270	= 3

-- TODO set the connectors
-- TODO wrap repeated code in nice utility functions

function part_new(arg1, arg2, arg3)
	local name, filename, connectors
	local spritefiles

	if(arg1 and arg2 and arg3) then
		assert(type(arg1) == "string" and type(arg2) == "string" and type(arg3) == "table")

		name = arg1
		filestub = "res/" .. arg2
		connectors = arg3
	else
		assert(type(arg1) == "string" and type(arg2) == "table")

		name = arg1
		filestub = "res/" .. arg1
		connectors = arg2
	end

	-- SPRITE FILENAMES
	if(satlib.file_exists(filestub .. ".bmp") == true) then
		-- only one
		spritefiles = {
			filestub,
			filestub,
			filestub,
			filestub,
		}
	else
		-- one for every 90 degrees
		spritefiles = {
			filestub .. "_0",
			filestub .. "_90",
			filestub .. "_180",
			filestub .. "_270",
		}
	end

	-- PART
	satlib.part_type_new(name)

	-- SPRITES
	for rotation, filename in pairs(spritefiles) do
		satlib.part_type_add_sprite(name, filename..".bmp", rotation)
	end

	-- CONNECTIONS
	if(#connectors > 0) then
		satlib.part_type_init_connectors(name, #connectors);

		for i, connection in pairs(connectors) do
			satlib.part_type_set_connector(name, i-1, connection.x, connection.y, connection.direction)
		end
	end
end

-- often used connection config for a single block
conn_one = {
	{ x = 0, y = 0, dir = LEFT },
	{ x = 0, y = 0, dir = TOP },
	{ x = 0, y = 0, dir = RIGHT },
	{ x = 0, y = 0, dir = DOWN }
}

part_new("base", conn_one)
part_new("mainframe", conn_one)
part_new("solar", {
	{ x = 0, y = 0, dir = LEFT }
})
part_new("wide dish", "dish_wide", {
	{ x = 0, y = 1, dir = LEFT }
})

print("parts loaded.")

