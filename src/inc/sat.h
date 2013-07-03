#ifndef SAT_H
#define SAT_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define SAT_TILE_WIDTH	8
#define SAT_TILE_HEIGHT	8

#define SAT_WIDTH	16
#define SAT_HEIGHT	16

#define SAT_N_PARTS	4

typedef enum {
	D0, D90, D180, D270
} Rot;

typedef enum {
	LEFT, UP, RIGHT, DOWN
} Dir;

typedef struct {
	int x, y;
} Pt;

struct Connector {
	Pt position;
	Dir direction;
};

struct Connection {
	struct Connector* child;
	struct Connector* parent;
};

struct SatPartInfo {
	char* name;
	struct SDL_Surface** sprites;
	unsigned int w;
	unsigned int h;
	struct Connector** connectors;
	unsigned int num_connectors;
};

struct SatPart {
	struct SatPart* parent;
	struct SatPartInfo* info;		// info about type of part
	struct Connection* connection;
	Rot rotation;
};

struct Satellite {
	char* name;
	long x, y;
	struct Node* parts; // node->data is SatPart*
};

void sat_render(struct Satellite* sat, struct SDL_Surface* canvas);
struct SatPart* sat_part_add(struct Satellite* sat, struct SatPartInfo* type, struct SatPart* parent, Rot rot, struct Connector* connParent, struct Connector* connChild);
void sat_init(lua_State* l);

extern struct Node* satellites;		//node->data is Satellite*
extern struct Node* sat_partinfos;	//node->data is SatPartInfo*

#endif
