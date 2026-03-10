#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(void) {
  int width, height, channels;

  std::string filename;

  unsigned char *data =
    stbi_load("assets/1.jpeg", &width, &height, &channels, 0);

  if (!data) {
    const char *reason = stbi_failure_reason();
    std::cerr << reason << std::endl;
    return 1;
  }

  std::cout << "image loaded with width = " << width
            << " and height = " << height << std::endl;

  for (int i{0}; i < 100; ++i) {
    int r = data[i * channels + 0];
    int g = data[i * channels + 1];
    int b = data[i * channels + 2];

    std::cerr << "Pixel " << i << ": R=" << r << "   G=" << g << "   B=" << b
              << std::endl;
  }
  std::cout << std::endl;

  stbi_image_free(data);

  return 0;
}
