#ifndef UTIL_H
#define UTIL_H

int rot_x(int x, int y, Rot r);
int rot_y(int x, int y, Rot r);
int rot_x_pt(Pt* p, Rot r);
int rot_y_pt(Pt* p, Rot r);

Pt sprite_raster(unsigned int x, unsigned int y, unsigned int index);

struct SatPartInfo* info_by_name(char* name);
struct Satellite* sat_by_name(char* name);
struct Connector* connector_by_sig(struct SatPartInfo* info, int x, int y, Dir dir);

void append_part(struct Satellite* sat, struct SatPart* part);
struct SatPart* get_last_part(struct Satellite* sat);

#endif
