#ifndef SAT_H
#define SAT_H

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

typedef enum {
	empty,
	base,
	mainframe,
	solar,
	dish_wide
} SatPartID;

typedef struct {
	int x, y;
} Pt;

struct Connector {
	Pt position;
	Dir direction;
};

struct SatPartInfo {
	struct SDL_Surface** sprites;
	unsigned int w;
	unsigned int h;
	struct Connector** connectors;
	unsigned int num_connectors;
};

struct SatPart {
	struct SatPart* parent;
	SatPartID id;			// the type of part
	int attachParent;	// where on parent to attach
	int attachLocal;			// where on part to attach to parent
	Rot rotation;
};

struct SatPartNode {
	struct SatPart* part;
	struct SatPartNode* next;
};

struct Satellite {
	long x, y;
	struct SatPartNode* first_part;
};

void sat_render(struct Satellite* sat, struct SDL_Surface* canvas);
void sat_init(void);

extern struct SatPartInfo* sat_parts[SAT_N_PARTS];
extern struct Satellite* satellites[4];

#endif
