#include <stddef.h>
#include <stdlib.h>

#include "satellite.h"
#include "sat_util.h"

int rot_x_pt(Pt* p, Rot r) {
	return rot_x(p->x, p->y, r);
}

int rot_y_pt(Pt* p, Rot r) {
	return rot_y(p->x, p->y, r);
}

int rot_x(int x, int y, Rot r) {
	switch(r) {
		case D0:
			return x;
			break;
		case D90:
			return -y;
			break;
		case D180:
			return -x;
			break;
		case D270:
			return y;
			break;
	}

	return 0;
}

int rot_y(int x, int y, Rot r) {
	switch(r) {
		case D0:
			return y;
			break;
		case D90:
			return x;
			break;
		case D180:
			return -y;
			break;
		case D270:
			return -x;
			break;
	}

	return 0;
}

Pt sprite_raster(unsigned int w, unsigned int h, unsigned int index) {
	int y = index/w;
	int x = index-y*w;
	return (Pt){ x, y };
}

// adds a part to the end of the linked list of all parts for the sat
void append_part(struct Satellite* sat, struct SatPart* part) {
	struct SatPartNode* last = sat->first_part;
	
	// construct the new node
	struct SatPartNode* newNode = (struct SatPartNode*)malloc(sizeof(struct SatPartNode));
	newNode->part = part;
	newNode->next = NULL;

	if(last == NULL) {
		sat->first_part = newNode;
	} else {
		// find the last node
		while(last->next != NULL)
			last = last->next;
		last->next = newNode;
	}
}

struct SatPart* get_last_part(struct Satellite* sat) {
	struct SatPartNode* last = sat->first_part;

	// find the last node
	while(last->next != NULL)
		last = last->next;

	return last->part;
}
