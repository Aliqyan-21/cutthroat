#include "preprocessing.h"

std::vector<std::vector<float>> make_grayscale(const ImgData &id) {
  /*
   * from `ITU-R BT.601` specification
   * gray = 0.299 * R + 0.587 * G + 0.114 * B
   */
  std::vector<std::vector<float>> gray(id.height, std::vector<float>(id.width));
  for (int y{0}; y < id.height; ++y) {
    for (int x{0}; x < id.width; ++x) {
      int i = (y * id.width + x) * id.channels;
      if (3 <= id.channels) {
        int r = id.pixels[i + 0];
        int g = id.pixels[i + 1];
        int b = id.pixels[i + 2];

        gray[y][x] = r * 0.299 + g * 0.587 + b * 0.114;
      } else {
        gray[y][x] = id.pixels[i];
      }
    }
  }
  return gray;
}
