#!/usr/bin/lua

-- require 'constants'

LEFT	= 0
UP		= 1
RIGHT	= 2
DOWN	= 3

TOP = UP
BOTTOM = DOWN

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
		satlib.part_type_add_sprite(name, filename..".bmp", rotation-1)
	end

	-- CONNECTIONS
	if(#connectors > 0) then
		satlib.part_type_init_connectors(name, #connectors);

		for i, connection in pairs(connectors) do
			satlib.part_type_set_connector(name, i-1, connection.x, connection.y, connection.dir)
		end
	end
end

-- recursively construct satellite
function expand(structure, depth)
	for key, value in pairs(structure) do
		if(type(value) == "table") then
			if(depth == nil) then
				expand(value, 1)
			else
				expand(value, depth+1)
			end
		else
			offset = ""
			if(depth ~= nil) then
				for i=0, depth do
					offset = offset.."\t"
				end
			end

			print(offset..value)
		end
	end
end

function sat_new(name, structure)
	assert(type(name) == "string" and type(structure) == "table")
	
	satlib.sat_new(name)
	expand(structure);
end

-- often used connection config for a single block
conn_one = {
	{ x = 0, y = 0, dir = LEFT },
	{ x = 0, y = 0, dir = TOP },
	{ x = 0, y = 0, dir = RIGHT },
	{ x = 0, y = 0, dir = DOWN }
}

-- CREATE PARTS
part_new("base", conn_one)
part_new("mainframe", conn_one)
part_new("solar", {
	{ x = 0, y = 0, dir = LEFT }
})
part_new("wide dish", "dish_wide", {
	{ x = 0, y = 1, dir = LEFT }
})

print("parts loaded.")

-- CREATE A TEST SATELLITE

satlib.sat_new("test", 25, 15)
satlib.sat_select("test")

function staircase()
	for i=1, 10 do
		for i=1, 3 do
			satlib.sat_part_add("base", 0, 0, LEFT, D0, 0, 0, RIGHT)
		end

		satlib.sat_part_add("solar", 0, 0, LEFT, D0, 0, 0, RIGHT)
		satlib.sat_part_last()
		satlib.sat_part_add("base", 0, 0, TOP, D0, 0, 0, BOTTOM)
		satlib.sat_part_add("base", 0, 0, TOP, D0, 0, 0, BOTTOM)
	end
end

function lineSat(dx, dy)
	if(dx ~= 0) then
		if(dx < 0) then
			for i=1, -dx do
				satlib.sat_part_add("base", 0, 0, RIGHT, D0, 0, 0, LEFT)
			end

			satlib.sat_part_add("wide dish", 0, 1, LEFT, D180, 0, 0, LEFT)
			satlib.sat_part_last()
		elseif(dx > 0) then
			for i=1, dx do
				satlib.sat_part_add("base", 0, 0, LEFT, D0, 0, 0, RIGHT)
			end

			satlib.sat_part_add("wide dish", 0, 1, LEFT, D0, 0, 0, RIGHT)
			satlib.sat_part_last()
		end
	else
		if(dy < 0) then
			for i=1, -dy do
				satlib.sat_part_add("base", 0, 0, BOTTOM, D0, 0, 0, TOP)
			end

			satlib.sat_part_add("wide dish", 0, 1, LEFT, D270, 0, 0, TOP)
			satlib.sat_part_last()
		elseif(dy > 0) then
			for i=1, dy do
				satlib.sat_part_add("base", 0, 0, TOP, D0, 0, 0, BOTTOM)
			end

			satlib.sat_part_add("wide dish", 0, 1, LEFT, D90, 0, 0, BOTTOM)
			satlib.sat_part_last()
		end
	end
end

function HilbertA(level, g, d)
  if level > depth then
    HilbertB(level-1,g,d);
    lineSat(0,d);
    HilbertA(level-1,g,d);
    lineSat(d,0);
    HilbertA(level-1,g,d);
    lineSat(0,-d);
    HilbertC(level-1,g,d);
  end
end

function HilbertB(level, g, d)
  if level > depth then
    HilbertA(level-1,g,d);
    lineSat(d,0);
    HilbertB(level-1,g,d);
    lineSat(0,d);
    HilbertB(level-1,g,d);
    lineSat(-d,0);
    HilbertD(level-1,g,d);
  end
end

function HilbertC(level, g, d)
  if level > depth then
    HilbertD(level-1,g,d);
    lineSat(-d,0);
    HilbertC(level-1,g,d);
    lineSat(0,-d);
    HilbertC(level-1,g,d);
    lineSat(d,0);
    HilbertA(level-1,g,d);
  end
end

function HilbertD(level, g, d)
  if level > depth then
    HilbertC(level-1,g,d);
    lineSat(0,-d);
    HilbertD(level-1,g,d);
    lineSat(-d,0);
    HilbertD(level-1,g,d);
    lineSat(0,d);
    HilbertB(level-1,g,d);
  end
end

depth = 6
level = 9
dist0 = 4096
dist = dist0
for i=0, level do
	dist = dist / 2;
	print(dist)
end
HilbertA(level, sg, dist); -- start recursion

-- staircase()
-- fractal(4)

--[[
sat_new("test", {
	type = "mainframe",
	cdir = LEFT,
	pdir = LEFT,
	parts = {
		{
			type = "base",
			cdir = LEFT,
			pdir = RIGHT,
			parts = {}
		},
		{
			type = "base",
			cdir = BOTTOM,
			pdir = TOP,
			parts = {}
		}
	}
})
--]]

print("test satellite created.")
