#ifndef __MODEL_H__
#define __MODEL_H__

#include "geometry/geometry.h"
#include <vector>

namespace rengen::io {

using namespace rengen::geometry;

class Face {
public:
  std::vector<int> vertIdx_;
  std::vector<int> tvertIdx_;
};

class ObjFile {
private:
  std::vector<Vec3f> verts_;
  std::vector<Face> faces_;
  std::vector<Vec3f> tverts_;

public:
  ObjFile(const char *filename);
  ~ObjFile();
  int nverts();
  int ntverts();
  int nfaces();
  Vec3f vert(int i);
  Vec3f tvert(int i);
  Face &face(int idx);
};
} // namespace rengen::io
#endif //__MODEL_H__