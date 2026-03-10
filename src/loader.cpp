#include "loader.h"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImgData load_image(const std::string &file_path) {
  ImgData id;
  unsigned char *data =
    stbi_load(file_path.c_str(), &id.width, &id.height, &id.channels, 0);
  if (!data) { throw std::runtime_error(stbi_failure_reason()); }
  int total = id.width * id.height * id.channels;
  id.pixels = std::vector<unsigned char>(data, data + total);
  stbi_image_free(data);
  return id;
}
