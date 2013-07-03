#ifndef UTIL_H
#define UTIL_H

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
