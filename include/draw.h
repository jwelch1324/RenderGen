#ifndef DRAW_H_
#define DRAW_H_
#include "tgaimage.h"
#include "geometry.h"
#include <math.h>
#include <algorithm>
namespace draw
{

    struct Triangle
    {
        std::vector<Vec3f> _verts;
        std::vector<Vec3f> _tverts;
    };

    auto white = TGAColor(255, 255, 255, 255);
    void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
    {
        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1))
        {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx = x1 - x0;
        int dy = y1 - y0;
        int derror2 = std::abs(dy) * 2;
        int error2 = 0;
        int y = y0;
        for (int x = x0; x <= x1; x++)
        {
            if (steep)
            {
                image.set(y, x, color);
            }
            else
            {
                image.set(x, y, color);
            }
            error2 += derror2;
            if (error2 > dx)
            {
                y += (y1 > y0 ? 1 : -1);
                error2 -= dx * 2;
            }
        }
    }
    void line(Vec2i t0, Vec2i t1, TGAImage &image, TGAColor &color)
    {
        line(t0.x, t0.y, t1.x, t1.y, image, color);
    }
    void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
    {
        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1))
        {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1)
        {
            // make it render left to right
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        auto div = (float)(x1 - x0);
        for (int x = x0; x <= x1; x++)
        {
            float t = (x - x0) / div;
            int y = y0 * (1.0 - t) + y1 * t;
            // printf("x:%d y:%d\n", x, y);
            if (steep)
            {
                image.set(y, x, color);
            }
            else
            {
                image.set(x, y, color);
            }
        }
    }
    void triangleold(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor &color)
    {
        line(t0, t1, image, color);
        line(t1, t2, image, color);
        line(t2, t0, image, color);
    }

    void trianglesweep(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor &color)
    {
        // Sort the vertices by y coordinate lowest to highest
        // if (t0.y > t1.y)
        //     std::swap(t0, t1);
        // if (t0.y > t2.y)
        //     std::swap(t1, t2);
        // if (t1.y > t2.y)
        //     std::swap(t0, t2);

        std::vector<Vec2i> points{t0, t1, t2};
        std::sort(points.begin(), points.end(),
                  [](Vec2i a, Vec2i b)
                  { return a.y < b.y; });
        t0 = points[0];
        t1 = points[1];
        t2 = points[2];

        // Rasterize the lower half of the triangle left and right sides
        // t0 has the lowest y coordinate, t1 the second lowest, and t2 the highest
        // The halfway point is the y coordinate of t1
        float dh = t2.y - t0.y;
        float db = t1.y - t0.y + 1;
        auto v0 = (t2 - t0); // Vector from t0 to t2
        auto v1 = (t1 - t0); // Vector from t0 to t1
        for (int y = t0.y; y <= t1.y; y++)
        {
            // Rasterize the left and right sides at the same time
            auto A = t0 + v0 * (float)((y - t0.y) / dh);
            auto B = t0 + v1 * (float)((y - t0.y) / db);
            image.set(A.x, y, color);
            image.set(B.x, y, color);
            // Draw a line between the boundary points
            if (A.x > B.x)
                std::swap(A, B);
            for (int j = A.x; j <= B.x; j++)
            {
                image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
            }
        }

        db = t2.y - t1.y + 1;
        v1 = (t2 - t1);
        for (int y = t1.y; y <= t2.y; y++)
        {
            // Rasterize left and right
            auto A = t2 + v0 * (float)((y - t2.y) / dh);
            auto B = t2 + v1 * (float)((y - t2.y) / db);
            image.set(A.x, y, color);
            image.set(B.x, y, color);
            if (A.x > B.x)
                std::swap(A, B);
            for (int j = A.x; j <= B.x; j++)
            {
                image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
            }
        }
    }

    inline void triangleBoundingBox(Vec2i &A, Vec2i &B, Vec2i &C, int *bbox_out)
    {
        // Bbox will be in the form [minx, miny, maxx, maxy]
        bbox_out[0] = std::min(std::min(A.x, B.x), C.x);
        bbox_out[1] = std::min(std::min(A.y, B.y), C.y);
        bbox_out[2] = std::max(std::max(A.x, B.x), C.x);
        bbox_out[3] = std::max(std::max(A.y, B.y), C.y);
    }

    inline void triangleBoundingBox3D(Vec3f &A, Vec3f &B, Vec3f &C, int *bbox_out, Vec2f clamp)
    {
        // Bbox will be in the form [minx, miny, maxx, maxy]
        bbox_out[0] = std::max(0.f, std::min(std::min(A.x, B.x), C.x));
        bbox_out[1] = std::max(0.f, std::min(std::min(A.y, B.y), C.y));
        bbox_out[2] = std::min(clamp.x, std::max(std::max(A.x, B.x), C.x));
        bbox_out[3] = std::min(clamp.y, std::max(std::max(A.y, B.y), C.y));
    }

    template <class VT>
    Vec3f barycentric(VT *points, Vec2i P)
    {
        // Get the cross product X^Y from the barycentric decomposition
        Vec3f u = Vec3f(points[2].x - points[0].x, points[1].x - points[0].x, points[0].x - P.x) ^ Vec3f(points[2].y - points[0].y, points[1].y - points[0].y, points[0].y - P.y);
        // If abs(u.z) < 1, then u.z < 0 -- return negative coordinate vector
        if (std::abs(u.z) < 1)
            return Vec3f(-1, 1, 1);

        // Otherwise we can calculate the barycentric coordinate vector [u v 1]
        return Vec3f(1.f - (u.x + u.y) / u.z, u.x / u.z, u.y / u.z);
    }

    Vec3f barycentricf(Vec3f *points, Vec2f P)
    {
        // Get the cross product X^Y from the barycentric decomposition
        Vec3f u = Vec3f(points[2].x - points[0].x, points[1].x - points[0].x, points[0].x - P.x) ^ Vec3f(points[2].y - points[0].y, points[1].y - points[0].y, points[0].y - P.y);
        // If abs(u.z) < 1, then u.z < 0 -- return negative coordinate vector
        if (std::abs(u.z) < 1)
            return Vec3f(-1, 1, 1);

        // Otherwise we can calculate the barycentric coordinate vector [u v 1]
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    }

    void trianglestd(Vec2i A, Vec2i B, Vec2i C, TGAImage &image, TGAColor &color)
    {
        // Sort by Y coordinate -- Not sure if this is still needed here...
        std::vector<Vec2i> points{A, B, C};
        std::sort(points.begin(), points.end(),
                  [](Vec2i a, Vec2i b)
                  { return a.y < b.y; });
        A = points[0];
        B = points[1];
        C = points[2];

        auto width = image.width();

        // We are going to scan the bounding box of the triangle so we need a way to find the bounding box...
        int bbox[4];
        memset(bbox, 0, 4);
        triangleBoundingBox(A, B, C, bbox);

        // Scanning each point in the bounding box
        for (int xc = bbox[0]; xc <= bbox[2]; xc++)
        {
            for (int yc = bbox[1]; yc <= bbox[3]; yc++)
            {
                // create a vec2i for the target point
                Vec2i P(xc, yc);
                auto z = barycentric(points.data(), P);

                // If the point doesn't fall within the triangle don't render
                if (z.x < 0 || z.y < 0 || z.z < 0)
                    continue;

                image.set(xc, yc, color);
            }
        }
    }

    void texturedTriangle(Triangle tri, float *zbuffer, TGAImage &image, TGAImage &texture, float ivec = 1.0)
    {
        auto A = tri._verts[0];
        auto B = tri._verts[1];
        auto C = tri._verts[2];

        auto width = image.width();
        auto height = image.height();

        int bbox[4];
        triangleBoundingBox3D(A, B, C, bbox, Vec2f(width - 1, height - 1));

        // Scanning each point in the bounding box
        for (int xc = bbox[0]; xc <= bbox[2]; xc++)
        {
            for (int yc = bbox[1]; yc <= bbox[3]; yc++)
            {
                // create a vec2i for the target point
                Vec2f P(xc, yc);
                auto bc_mask = barycentricf(tri._verts.data(), P);

                // If the point doesn't fall within the triangle don't render
                if (bc_mask.x < 0 || bc_mask.y < 0 || bc_mask.z < 0)
                    continue;

                // Now to determine if we draw this triangle we need to figure out if its z coordinate is acceptable.
                float zcoord = A.z * bc_mask.x + B.z * bc_mask.y + C.z * bc_mask.z;
                if (zbuffer[xc + yc * width] < zcoord)
                {
                    zbuffer[xc + yc * width] = zcoord;

                    // We need to interpolate the u,v coordinates in the texture map to get the color of this vertex.
                    float u = tri._tverts[0].x * bc_mask.x + tri._tverts[1].x * bc_mask.y + tri._tverts[2].x * bc_mask.z;
                    float v = tri._tverts[0].y * bc_mask.x + tri._tverts[1].y * bc_mask.y + tri._tverts[2].y * bc_mask.z;
                    TGAColor color = texture.get(u, v);
                    for (int l = 0; l < 3; l++)
                    {
                        color.bgra[l] = uint8_t(float(color.bgra[l]) * ivec);
                    }

                    image.set(xc, yc, color);
                }
            }
        }
    }

    void triangle(Vec3f A, Vec3f B, Vec3f C, float *zbuffer, TGAImage &image, TGAColor &color)
    {
        // Sort by Y coordinate -- Not sure if this is still needed here...
        std::vector<Vec3f> points{A, B, C};
        // std::sort(points.begin(), points.end(),
        //           [](Vec3f a, Vec3f b)
        //           { return a.y < b.y; });
        A = points[0];
        B = points[1];
        C = points[2];

        auto width = image.width();

        // We are going to scan the bounding box of the triangle so we need a way to find the bounding box...
        int bbox[4];
        memset(bbox, 0, 4);
        triangleBoundingBox3D(A, B, C, bbox, Vec2f(image.width() - 1, image.height() - 1));

        // Scanning each point in the bounding box
        for (int xc = bbox[0]; xc <= bbox[2]; xc++)
        {
            for (int yc = bbox[1]; yc <= bbox[3]; yc++)
            {
                // create a vec2i for the target point
                Vec2f P(xc, yc);
                auto bc_mask = barycentricf(points.data(), P);

                // If the point doesn't fall within the triangle don't render
                if (bc_mask.x < 0 || bc_mask.y < 0 || bc_mask.z < 0)
                    continue;

                // Now to determine if we draw this triangle we need to figure out if its z coordinate is acceptable.
                float zcoord = A.z * bc_mask.x + B.z * bc_mask.y + C.z * bc_mask.z;
                if (zbuffer[xc + yc * width] < zcoord)
                {
                    zbuffer[xc + yc * width] = zcoord;
                    image.set(xc, yc, color);
                }
            }
        }
    }
    const int depth = 255;
    Matrix viewport(int x, int y, int w, int h)
    {
        Matrix m = Matrix::identity(4);
        m[0][3] = x + w / 2.f;
        m[1][3] = y + h / 2.f;
        m[2][3] = depth / 2.f;

        m[0][0] = w / 2.f;
        m[1][1] = h / 2.f;
        m[2][2] = depth / 2.f;
        return m;
    }

    void renderHeadTexturedProjective()
    {
        auto width = 1000;
        auto height = 1000;
        Vec3f lightVec(0, 0, -1);
        TGAImage image(width, height, TGAImage::RGB);

        TGAImage tex = TGAImage();
        tex.read_tga_file("african_head_diffuse.tga");
        tex.flip_vertically();

        auto twidth = tex.width();
        auto theight = tex.height();

        float *zbuffer = new float[height * width];
        for (int i = 0; i < height * width; i++)
        {
            zbuffer[i] = -std::numeric_limits<float>::max();
        }

        auto model = Model("african_head.obj");

        Matrix proj(4, 4);
        proj[0][0] = 1;
        proj[1][1] = 1;
        proj[2][2] = 1;
        proj[3][2] = -1.f / 2;
        proj[3][3] = 1;

        // Matrix proj = Matrix::identity(4);
        // proj[3][2] = -1.f/3.0f;

        Matrix vp = viewport(image.width()/8, image.height()/8, image.width() * 3 / 4, image.height() * 3 / 4);

        for (int i = 0; i < model.nfaces(); i++)
        {
            //  printf("\r%d : %d", i, model.nfaces());
            // Get the face we are going to draw the wireframe for
            Face face = model.face(i);
            // Face definition format -- first number in each group is a vertex making up the triangle.
            // f 1193/1240/1193 1180/1227/1180 1179/1226/1179
            // Now we loop through the vertices in each face and draw the lines connecting all the vertices (which are in x,y,z format)
            Vec3f t[3];
            Triangle tri = draw::Triangle();
            for (int j = 0; j < 3; j++)
            {
                Vec3f v0 = model.vert(face.vertIdx_[j]);
                // Vec3f vtmp = Vec3f();
                // vtmp.x = (v0.x + 1) * width / 2.;
                // vtmp.y = (v0.y + 1) * height / 2.;
                // vtmp.z = (v0.z);

                Vec3f xx = v0;
                Matrix tmp(4, 1);
                tmp[0][0] = xx.x;
                tmp[1][0] = xx.y;
                tmp[2][0] = xx.z;
                tmp[3][0] = 1;

                Matrix res =  vp*proj *tmp;
                float zz = res[3][0];
                Vec3f newpoint(res[0][0] / zz, res[1][0] / zz, res[2][0] / zz);
                printf("%f,%f,%f\n",newpoint.x,newpoint.y,newpoint.z);
                tri._verts.push_back(newpoint);

                Vec3f texVerts = model.tvert(face.tvertIdx_[j]);
                tri._tverts.push_back(Vec3f(texVerts.x * twidth, texVerts.y * theight, 0));
            }

            // Get the normal to the triangle
            Vec3f zz = (model.vert(face.vertIdx_[2]) - model.vert(face.vertIdx_[0])) ^ (model.vert(face.vertIdx_[1]) - model.vert(face.vertIdx_[0]));
            zz.normalize();

            // Get light intensity as scalar product of light vector with normal
            auto ivec = zz * lightVec;
            // printf("%f\n", ivec * 255);

            if (ivec > 0)
            {
                draw::texturedTriangle(tri, zbuffer, image, tex, ivec);
            }
        }

        image.write_tga_file("renderShadedTextureProjective.tga");
    }
}
#endif