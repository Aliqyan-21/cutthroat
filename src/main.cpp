#include <iostream>

#include "fft_detector.h"
#include "loader.h"

struct Args {
  std::string real_image_path;
  std::string ai_image_path;
};

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "usage: " << argv[0] << " <real image path> <ai image path>"
              << std::endl;
    return 1;
  }
  Args args;
  args.real_image_path = argv[1];
  args.ai_image_path   = argv[2];

  {
    auto d = load_image(args.real_image_path);
    FFTDetector fd(d);
    fd.calculate_fft();
    fd.calculate_aaps("real_aaps.dat");
  }

  {
    auto d = load_image(args.ai_image_path);
    FFTDetector fd(d);
    fd.calculate_fft();
    fd.calculate_aaps("ai_aaps.dat");
  }

  return 0;
}
