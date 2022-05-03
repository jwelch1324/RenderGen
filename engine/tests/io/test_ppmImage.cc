#include "io/ppmimage.h"
#include "io/tgaimage.h"
#include <gtest/gtest.h>

TEST(TestReadWritePPM, BasicAssertions) {
  // Use a TGA file as a test
  rengen::io::TGAImage img;
  img.read_tga_file("UmbreonEye.tga");

  int x = img.width();
  int y = img.height();

  rengen::io::PPMImage out(x, y);

  for (int ix = 0; ix < x; ix++) {
    for (int iy = 0; iy < y; iy++) {
      out.Set(ix, iy, img.get(ix, iy));
    }
  }

  out.Write("ppmtestout.ppm");

  // rengen::io::PPMImage newImg = rengen::io::PPMImage::Read("ppmtestout.ppm");

  // EXPECT_EQ(out,newImg);
}

TEST(WriteGradientPPM, BasicAssertions) {
  int nx = 200;
  int ny = 100;
  rengen::io::PPMImage img(nx, ny);
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      float r = float(i) / float(nx);
      float g = float(j) / float(ny);
      float b = 0.2;
      int ir = int(255.99 * r);
      int ig = int(255.99 * g);
      int ib = int(255.99 * b);
      img.Set(i, j, rengen::io::RGBAColor(ir, ig, ib));
    }
  }

  img.Write("gradientTest.ppm");
}
