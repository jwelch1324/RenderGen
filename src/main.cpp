#include <iostream>
#include <chrono>
#include "tgaimage.h"
#include "model.h"
#include "draw.h"

TGAColor white = TGAColor(255, 255, 255, 255);
auto red = TGAColor(255, 0, 0, 255);
auto green = TGAColor(0, 255, 0, 255);
TGAColor colors[3] = {white, red, green};
int main2(int, char **)
{
    auto width = 250;
    auto height = 250;
    TGAImage image(width, height, TGAImage::RGB);

    // draw::line(Vec2i(10,10),Vec2i(14,80),image,green);

    // draw::triangle(Vec2i(50, 50), Vec2i(14, 80), Vec2i(30, 92), image, red);
    // // draw::triangle(Vec2i(120,50),Vec2i(120,80),Vec2i(150,15),image,green);
    // draw::triangle(Vec2i(120, 50), Vec2i(120, 80), Vec2i(150, 30), image, green);
    // draw::triangle(Vec2i(120, 50), Vec2i(120, 80), Vec2i(150, 30), image, green);

    Vec2i t0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    draw::triangleold(t0[0], t0[1], t0[2], image, red);
    draw::triangleold(t1[0], t1[1], t1[2], image, white);
    draw::triangleold(t2[0], t2[1], t2[2], image, green);
    image.write_tga_file("triangle.tga");

    image = TGAImage(width, height, TGAImage::RGB);
    draw::trianglesweep(t0[0], t0[1], t0[2], image, red);
    draw::trianglesweep(t1[0], t1[1], t1[2], image, white);
    draw::trianglesweep(t2[0], t2[1], t2[2], image, green);
    image.write_tga_file("triangle2.tga");

    image = TGAImage(width, height, TGAImage::RGB);
    draw::trianglestd(t0[0], t0[1], t0[2], image, red);
    draw::trianglestd(t1[0], t1[1], t1[2], image, white);
    draw::trianglestd(t2[0], t2[1], t2[2], image, green);
    image.write_tga_file("triangle3.tga");

    return 0;
}

void renderHeadFlatFast()
{
    auto width = 1000;
    auto height = 1000;
    TGAImage image(width, height, TGAImage::RGB);

    auto model = Model("african_head.obj");

    for (auto i = 0; i < model.nfaces(); i++)
    {
        // printf("\r%d : %d", i, model.nfaces());
        //  Get the face we are going to draw the wireframe for
        auto face = model.face(i);

        // Face definition format -- first number in each group is a vertex making up the triangle.
        // f 1193/1240/1193 1180/1227/1180 1179/1226/1179
        // Now we loop through the vertices in each face and draw the lines connecting all the vertices (which are in x,y,z format)
        Vec2i t[3];
        for (int j = 0; j < 3; j++)
        {
            auto v0 = model.vert(face.vertIdx_[j]);
            t[j].x = (v0.x + 1) * width / 2.;
            t[j].y = (v0.y + 1) * height / 2.;
        }

        auto cc = TGAColor(rand() % 255, rand() % 255, rand() % 255, 255);
        draw::trianglestd(t[0], t[1], t[2], image, cc);
    }

    image.write_tga_file("render.tga");
}

void renderHeadShaded()
{
    auto width = 1000;
    auto height = 1000;
    Vec3f lightVec(0, 0, -1);
    TGAImage image(width, height, TGAImage::RGB);
    float *zbuffer = new float[height * width];
    for(int i = 0; i< height*width; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }

    auto model = Model("african_head.obj");

    for (auto i = 0; i < model.nfaces(); i++)
    {
        //  printf("\r%d : %d", i, model.nfaces());
        // Get the face we are going to draw the wireframe for
        auto face = model.face(i);

        // Face definition format -- first number in each group is a vertex making up the triangle.
        // f 1193/1240/1193 1180/1227/1180 1179/1226/1179
        // Now we loop through the vertices in each face and draw the lines connecting all the vertices (which are in x,y,z format)
        Vec3f t[3];
        for (int j = 0; j < 3; j++)
        {
            auto v0 = model.vert(face.vertIdx_[j]);
            t[j].x = (v0.x + 1) * width / 2.;
            t[j].y = (v0.y + 1) * height / 2.;
            t[j].z = (v0.z);
        }

        // Get the normal to the triangle
        auto zz = (model.vert(face.vertIdx_[2]) - model.vert(face.vertIdx_[0])) ^ (model.vert(face.vertIdx_[1]) - model.vert(face.vertIdx_[0]));
        zz.normalize();

        // Get light intensity as scalar product of light vector with normal
        auto ivec = zz * lightVec;
        // printf("%f\n", ivec * 255);
        if (ivec > 0)
        {
            auto cc = TGAColor(ivec * 255, ivec * 255, ivec * 255, 255);
            draw::triangle(t[0], t[1], t[2], zbuffer, image, cc);
        }
    }

    image.write_tga_file("renderShaded.tga");
}

void renderHeadFlatSweep()
{
    auto width = 1000;
    auto height = 1000;
    TGAImage image(width, height, TGAImage::RGB);

    auto model = Model("african_head.obj");

    for (auto i = 0; i < model.nfaces(); i++)
    {
        printf("\r%d : %d", i, model.nfaces());
        // Get the face we are going to draw the wireframe for
        auto face = model.face(i);

        // Face definition format -- first number in each group is a vertex making up the triangle.
        // f 1193/1240/1193 1180/1227/1180 1179/1226/1179
        // Now we loop through the vertices in each face and draw the lines connecting all the vertices (which are in x,y,z format)
        Vec2i t[3];
        for (int j = 0; j < 3; j++)
        {
            auto v0 = model.vert(face.vertIdx_[j]);
            t[j].x = (v0.x + 1) * width / 2.;
            t[j].y = (v0.y + 1) * height / 2.;
        }

        draw::trianglesweep(t[0], t[1], t[2], image, colors[i % 3]);
    }
    // image.write_tga_file("render.tga");
}

void renderHeadWireframe(int, char **)
{
    auto width = 1000;
    auto height = 1000;
    TGAImage image(width, height, TGAImage::RGB);

    auto model = Model("african_head.obj");

    // For each face in the mesh we want to draw the lines which make up the border of the triangle
    for (auto i = 0; i < model.nfaces(); i++)
    {
        // Get the face we are going to draw the wireframe for
        auto face = model.face(i);

        // Face definition format -- first number in each group is a vertex making up the triangle.
        // f 1193/1240/1193 1180/1227/1180 1179/1226/1179
        // Now we loop through the vertices in each face and draw the lines connecting all the vertices (which are in x,y,z format)
        for (auto j = 0; j < 3; j++)
        {
            auto v0 = model.vert(face.vertIdx_[j]);
            auto v1 = model.vert(face.vertIdx_[(j + 1) % 3]); // We take mod 3 to always pick a vertex we haven't connected to yet so v0 @j=0 ==> v1 @(j+1)%3=1; v0 @j=2 ==> v1 @(j+2)%3 = 0
            int x0 = (v0.x + 1) * width / 2.;
            int y0 = (v0.y + 1) * height / 2.;
            int x1 = (v1.x + 1) * width / 2.;
            int y1 = (v1.y + 1) * height / 2.;
            draw::line(x0, y0, x1, y1, image, white);
        }
    }
    printf("\n");

    // image.write_tga_file("output.tga");
}

void renderHeadTextured()
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
    for(int i = 0; i< height*width; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }

    auto model = Model("african_head.obj");

    for (auto i = 0; i < model.nfaces(); i++)
    {
        //  printf("\r%d : %d", i, model.nfaces());
        // Get the face we are going to draw the wireframe for
        auto face = model.face(i);
        // Face definition format -- first number in each group is a vertex making up the triangle.
        // f 1193/1240/1193 1180/1227/1180 1179/1226/1179
        // Now we loop through the vertices in each face and draw the lines connecting all the vertices (which are in x,y,z format)
        Vec3f t[3];
        auto tri = draw::Triangle();
        for (int j = 0; j < 3; j++)
        {
            auto v0 = model.vert(face.vertIdx_[j]);
            tri._verts.push_back(Vec3f());
            tri._verts[j].x = (v0.x + 1) * width / 2.;
            tri._verts[j].y = (v0.y + 1) * height / 2.;
            tri._verts[j].z = (v0.z);

            auto texVerts = model.tvert(face.tvertIdx_[j]);
            tri._tverts.push_back(Vec3f(texVerts.x*twidth, texVerts.y*theight,0));
        }




        // Get the normal to the triangle
        auto zz = (model.vert(face.vertIdx_[2]) - model.vert(face.vertIdx_[0])) ^ (model.vert(face.vertIdx_[1]) - model.vert(face.vertIdx_[0]));
        zz.normalize();

        // Get light intensity as scalar product of light vector with normal
        auto ivec = zz * lightVec;
        // printf("%f\n", ivec * 255);


        if (ivec > 0)
        {
            auto cc = TGAColor(ivec * 255, ivec * 255, ivec * 255, 255);
            //draw::triangle(t[0], t[1], t[2], zbuffer, image, cc);
            draw::texturedTriangle(tri,zbuffer,image,tex, ivec);
        }
    }

    image.write_tga_file("renderShadedTexture.tga");
}


int main(int argc, char **argv)
{
    // //main2(argc, argv);
    // auto s = std::chrono::high_resolution_clock::now();
    // renderHeadFlatSweep();
    // auto e = std::chrono::high_resolution_clock::now();
    // printf("Took %lld microseconds for Sweep Render\n", std::chrono::duration_cast<std::chrono::microseconds>(e - s).count());

    auto s = std::chrono::high_resolution_clock::now();
    draw::renderHeadTexturedProjective();
    auto e = std::chrono::high_resolution_clock::now();
    printf("Took %lld microseconds for Fast Render\n", std::chrono::duration_cast<std::chrono::microseconds>(e - s).count());
}