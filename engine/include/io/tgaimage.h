#pragma once
#include <cstdint>
#include <fstream>
#include <sys/types.h>
#include <vector>

namespace rengen::io {

#pragma pack(push, 1)
struct TGAHeader {
  std::uint8_t idlength{};
  std::uint8_t colormaptype{};
  std::uint8_t datatypecode{};
  std::uint16_t colormaporigin{};
  std::uint16_t colormaplength{};
  std::uint8_t colormapdepth{};
  std::uint16_t x_origin{};
  std::uint16_t y_origin{};
  std::uint16_t width{};
  std::uint16_t height{};
  std::uint8_t bitsperpixel{};
  std::uint8_t imagedescriptor{};
};
#pragma pack(pop)

struct RGBAColor {
public:
  uint8_t r() const { return bgra[2]; }
  uint8_t g() const { return bgra[1]; }
  uint8_t b() const { return bgra[0]; }
  uint8_t a() const { return bgra[3]; }

  void r(uint8_t c) { bgra[2] = c; }
  void g(uint8_t c) { bgra[1] = c; }
  void b(uint8_t c) { bgra[0] = c; }
  void a(uint8_t c) { bgra[3] = c; }

  std::uint8_t bgra[4] = {0, 0, 0, 0};
  std::uint8_t bytespp = {0};

  RGBAColor() = default;
  RGBAColor(const std::uint8_t R, const std::uint8_t G, const std::uint8_t B,
            const std::uint8_t A = 255)
      : bgra{B, G, R, A}, bytespp(4) {}
  RGBAColor(const std::uint8_t *p, const std::uint8_t bpp) : bytespp(bpp) {
    for (int i = bpp; i--; bgra[i] = p[i])
      ;
  }
  std::uint8_t &operator[](const int i) { return bgra[i]; }

  bool operator==(const RGBAColor &o) const {
    for (int i = 0; i < 4; i++)
      if (bgra[i] != o.bgra[i])
        return false;

    return true;
  }

  bool operator!=(const RGBAColor &o) const {
      return !(*this == o);
  }
};

struct TGAImage {
  enum Format { GRAYSCALE = 1, RGB = 3, RGBA = 4 };

  TGAImage() = default;
  TGAImage(const int w, const int h, const int bpp);
  bool read_tga_file(const std::string filename);
  bool write_tga_file(const std::string filename, const bool vflip = true,
                      const bool rle = true) const;
  void flip_horizontally();
  void flip_vertically();
  RGBAColor get(const int x, const int y) const;
  void set(const int x, const int y, const RGBAColor &c);
  int width() const;
  int height() const;

private:
  bool load_rle_data(std::ifstream &in);
  bool unload_rle_data(std::ofstream &out) const;

  int w = 0;
  int h = 0;
  int bpp = 0;
  std::vector<std::uint8_t> data = {};
};

} // namespace rengen::io