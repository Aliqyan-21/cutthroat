#include <iostream>

#include "loader.h"
#include "preprocessing.h"

struct Args {
  std::string file_path;
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <file path>" << std::endl;
    return 1;
  }
  Args args;
  args.file_path = argv[1];

  auto d = load_image(args.file_path);

  std::cout << "image loaded with width = " << d.width
            << " and height = " << d.height << std::endl;

  auto gray = make_grayscale(d);

  for (int i{0}; i < 10; ++i) {
    for (int j{0}; j < 10; ++j) { std::cout << gray[i][j] << std::endl; }
  }

  return 0;
}
