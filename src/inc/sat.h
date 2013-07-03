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

typedef struct Pt {
	int x, y;
} Pt;

typedef struct Connector {
	Pt position;
	Dir direction;
} Connector;

typedef struct Connection {
	Connector* child;
	Connector* parent;
} Connection;

typedef struct SatPartInfo {
	char* name;
	struct SDL_Surface** sprites;
	unsigned int w;
	unsigned int h;
	Connector** connectors;
	unsigned int num_connectors;
} SatPartInfo;

typedef struct SatPart {
	struct SatPart* parent;
	SatPartInfo* info;		// info about type of part
	Connection* connection;
	Rot rotation;
} SatPart;

typedef struct Satellite {
	char* name;
	long x, y;
	struct Node* parts; // node->data is SatPart*
} Satellite;

void sat_render(Satellite* sat, struct SDL_Surface* canvas);
SatPart* sat_part_add(Satellite* sat, SatPartInfo* type, SatPart* parent, Rot rot, Connector* connParent, Connector* connChild);
void sat_init(lua_State* l);

extern struct Node* satellites;		//node->data is Satellite*
extern struct Node* sat_partinfos;	//node->data is SatPartInfo*

#endif
