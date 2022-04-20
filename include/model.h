#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Face
{
	public:
	std::vector<int> vertIdx_;
	std::vector<int> tvertIdx_;
};
class Model
{
private:
	std::vector<Vec3f> verts_;
	std::vector<Face> faces_;
	std::vector<Vec3f> tverts_;

public:
	Model(const char *filename);
	~Model();
	int nverts();
	int ntverts();
	int nfaces();
	Vec3f vert(int i);
	Vec3f tvert(int i);
	Face &face(int idx);
};

#endif //__MODEL_H__