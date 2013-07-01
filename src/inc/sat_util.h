int rot_x(int x, int y, Rot r);
int rot_y(int x, int y, Rot r);
int rot_x_pt(Pt* p, Rot r);
int rot_y_pt(Pt* p, Rot r);

Pt sprite_raster(unsigned int x, unsigned int y, unsigned int index);

void append_part(struct Satellite* sat, struct SatPart* part);
struct SatPart* get_last_part(struct Satellite* sat);
