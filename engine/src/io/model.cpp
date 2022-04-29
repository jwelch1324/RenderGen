#include "io/obj_file.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace rengen::io {
ObjFile::ObjFile(const char *filename) : verts_(), faces_() {
  std::ifstream in;
  in.open(filename, std::ifstream::in);
  if (in.fail()) {
    printf("failed to open file %s\n", filename);
    return;
  }
  std::string line;
  while (!in.eof()) {
    std::getline(in, line);
    std::istringstream iss(line.c_str());
    char trash;
    if (!line.compare(0, 2, "v ")) {
      iss >> trash;
      Vec3f v;
      for (int i = 0; i < 3; i++)
        iss >> v[i];
      verts_.push_back(v);
    } else if (!line.compare(0, 2, "f ")) {
      Face f;
      int itrash, idx, vtidx;
      iss >> trash;
      while (iss >> idx >> trash >> vtidx >> trash >> itrash) {
        idx--; // in wavefront obj all indices start at 1, not zero
        vtidx--;
        f.vertIdx_.push_back(idx);
        f.tvertIdx_.push_back(vtidx);
      }
      faces_.push_back(f);
    } else if (!line.compare(0, 2, "vt")) {
      iss >> trash >> trash;
      Vec3f vt;
      for (int i = 0; i < 3; i++) {
        iss >> vt[i];
      }

      tverts_.push_back(vt);
    }
  }
  std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# "
            << tverts_.size() << std::endl;
}

ObjFile::~ObjFile() {}

int ObjFile::nverts() { return (int)verts_.size(); }

int ObjFile::nfaces() { return (int)faces_.size(); }

int ObjFile::ntverts() { return (int)tverts_.size(); }

Face &ObjFile::face(int idx) { return faces_[idx]; }

Vec3f ObjFile::vert(int i) { return verts_[i]; }

Vec3f ObjFile::tvert(int i) { return tverts_[i]; }
} // namespace regen::io