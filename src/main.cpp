#include <iostream>

#include "fft_detector.h"
#include "loader.h"

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

  FFTDetector fd(d);
  fd.run_detection();
  auto s1 = fd.get_spectrum_1d();
  auto s2 = fd.get_spectrum_2d();

  for (int i{0}; i < 10; ++i) {
    float mag = std::sqrtf(s1[i].real * s1[i].real + s1[i].imag * s1[i].imag);
    std::cout << "mag[" << i << "] = " << mag << std::endl;
  }

  std::cout << std::endl;

  for (int i{0}; i < 10; ++i) {
    float mag = std::sqrtf(s2[i].real * s2[i].real + s2[i].imag * s2[i].imag);
    std::cout << "mag[" << i << "] = " << mag << std::endl;
  }

  return 0;
}
