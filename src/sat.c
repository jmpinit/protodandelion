#include <stdbool.h>

#include "SDL/SDL.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "sat.h"
#include "sat_util.h"

#include "satlib/satlib.h"

#include "sprite.h"
#include "list.h"

struct Node* sat_partinfos;
struct Node* satellites;

bool connection_is_valid(struct Connection* conn) {
	bool valid = true;

	// check for compatible directions
	Dir dc = conn->child->direction;
	Dir dp = conn->parent->direction;

	if(dc == UP && dp == DOWN) {
	} else if(dc == DOWN && dp == UP) {
	} else if(dc == LEFT && dp == RIGHT) {
	} else if(dc == RIGHT && dp == LEFT) {
	} else {
		valid = false;
	}

	return valid;
}

struct SatPart* sat_part_add(struct Satellite* sat, struct SatPartInfo* type, struct SatPart* parent, Rot rot, struct Connector* connParent, struct Connector* connChild) {
	// create the part
	struct SatPart* part = calloc(1, sizeof(struct SatPart));

	// set the part fields
	part->parent = parent;
	part->info = type;

	part->connection = malloc(sizeof(struct Connection));
	part->connection->parent = connParent;
	part->connection->child = connChild;

	part->rotation = rot;

	// add the part to the satellite
	list_add(sat->parts, part);

	return part;
}

void sat_render(struct Satellite* sat, SDL_Surface* canvas) {
	struct Node* current = sat->parts;
	struct SatPart* part = (struct SatPart*)current->data;

	// render every part separately
	while(current != NULL) {
		// calculate offset by following tree to root part
		int dx = 0;
		int dy = 0;
		struct SatPart* child = part;

		while(child != NULL) {
			// gather connection information
			struct SatPart* parent = child->parent;

			struct Connector* connChild = child->connection->child;
			struct Connector* connParent = child->connection->parent;

			// offset based on connector location
			dx += rot_x_pt(&connParent->position, parent->rotation) -
				rot_x_pt(&connChild->position, child->rotation);
			dy += rot_y_pt(&connParent->position, parent->rotation) -
				rot_y_pt(&connChild->position, child->rotation);

			// offset based on connector directions
			Dir dc = (connChild->direction + child->rotation)%4;
			Dir dp = (connParent->direction + parent->rotation)%4;

			if(dc == UP && dp == DOWN) {
				dy += 1;
			} else if(dc == DOWN && dp == UP) {
				dy -= 1;
			} else if(dc == LEFT && dp == RIGHT) {
				dx += 1;
			} else if(dc == RIGHT && dp == LEFT) {
				dx -= 1;
			} else {
				printf("error: invalid connection.\n");
				exit(0);
			}

			child = parent;
		}

		// render sprite at offset
		sprite_render(canvas, part, sat->x + dx, sat->y + dy, part->rotation);

		current = current->next;
	}
}

void sat_init(lua_State* L) {
	sat_partinfos = calloc(1, sizeof(struct Node));
	satellites = calloc(1, sizeof(struct Node));

	// load the library
	openlualibs(L);

	// run the load script
	const char* startup = "src/scripts/load.lua";
	int s = luaL_loadfile(L, startup);

	if ( s==0 ) {
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
		if(s) fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
		printf("Called script.");
	} else {
		fprintf(stderr, "Lua load error: %s\n", lua_tostring(L, -1));
		exit(0);
	}
}
