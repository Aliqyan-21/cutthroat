#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <vector>

/*
 * Rules:
 * Class and Struct in PascalCase
 * functions,variables,etc in snake_case
 */

/* data to return for the function load_image(...) */
struct ImgData {
  int width, height, channels;
  std::vector<unsigned char> pixels;
};

/* loads the image and returns ImgData */
ImgData load_image(const std::string &file_path);

#endif  //! LOADER_H
