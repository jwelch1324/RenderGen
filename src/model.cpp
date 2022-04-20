#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_()
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
    {
        printf("failed to open file %s\n", filename);
        return;
    }
    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++)
                iss >> v.raw[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 2, "f "))
        {
            Face f;
            int itrash, idx, vtidx;
            iss >> trash;
            while (iss >> idx >> trash >> vtidx >> trash >> itrash)
            {
                idx--; // in wavefront obj all indices start at 1, not zero
                vtidx--;
                f.vertIdx_.push_back(idx);
                f.tvertIdx_.push_back(vtidx);
            }
            faces_.push_back(f);
        }
        else if (!line.compare(0, 2, "vt"))
        {
            iss >> trash >> trash;
            Vec3f vt;
            for (int i = 0; i < 3; i++)
            {
                iss >> vt.raw[i];
            }

            tverts_.push_back(vt);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# " << tverts_.size() << std::endl;
}

Model::~Model()
{
}

int Model::nverts()
{
    return (int)verts_.size();
}

int Model::nfaces()
{
    return (int)faces_.size();
}

int Model::ntverts()
{
    return (int)tverts_.size();
}

Face &Model::face(int idx)
{
    return faces_[idx];
}

Vec3f Model::vert(int i)
{
    return verts_[i];
}

Vec3f Model::tvert(int i)
{
    return tverts_[i];
}