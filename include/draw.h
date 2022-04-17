#ifndef DRAW_H_
#define DRAW_H_
#include "tgaimage.h"
#include "geometry.h"
namespace draw
{
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

    Vec3f barycentric(Vec2i *points, Vec2i P)
    {
        // Get the cross product X^Y from the barycentric decomposition
        Vec3f u = Vec3f(points[2].x - points[0].x, points[1].x - points[0].x, points[0].x - P.x) ^ Vec3f(points[2].y - points[0].y, points[1].y - points[0].y, points[0].y - P.y);
        // If abs(u.z) < 1, then u.z < 0 -- return negative coordinate vector
        if (std::abs(u.z) < 1)
            return Vec3f(-1, 1, 1);

        // Otherwise we can calculate the barycentric coordinate vector [u v 1]
        return Vec3f(1.f - (u.x + u.y) / u.z, u.x / u.z, u.y / u.z);
    }

    void triangle(Vec2i A, Vec2i B, Vec2i C, TGAImage &image, TGAColor &color)
    {
        // Sort by Y coordinate -- Not sure if this is still needed here...
        std::vector<Vec2i> points{A, B, C};
        std::sort(points.begin(), points.end(),
                  [](Vec2i a, Vec2i b)
                  { return a.y < b.y; });
        A = points[0];
        B = points[1];
        C = points[2];

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
}

#endif