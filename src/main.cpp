#include <iostream>

#include "loader.h"

int main(void) {
  auto d = load_image("assets/1.jpeg");

  std::cout << "image loaded with width = " << d.width
            << " and height = " << d.height << std::endl;

  for (int i{0}; i < 100; ++i) {
    int r = d.pixels[i * d.channels + 0];
    int g = d.pixels[i * d.channels + 1];
    int b = d.pixels[i * d.channels + 2];

    std::cerr << "Pixel " << i << ": R=" << r << "   G=" << g << "   B=" << b
              << std::endl;
  }
  std::cout << std::endl;

  return 0;
}
