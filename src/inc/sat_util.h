#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

bool compatible_directions(Dir a, Dir b);
bool connection_is_valid(Connection* conn, Rot a, Rot b);
bool connectable(Connector* a, Rot rotA, Connector* b, Rot rotB);

int rot_x(int x, int y, Rot r);
int rot_y(int x, int y, Rot r);
int rot_x_pt(Pt* p, Rot r);
int rot_y_pt(Pt* p, Rot r);

Pt sprite_raster(unsigned int x, unsigned int y, unsigned int index);

SatPartInfo* info_by_name(char* name);
Satellite* sat_by_name(char* name);
Connector* connector_by_sig(SatPartInfo* info, int x, int y, Dir dir);

void append_part(Satellite* sat, SatPart* part);
SatPart* get_last_part(Satellite* sat);

#endif
