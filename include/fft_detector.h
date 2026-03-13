#ifndef FFT_DETECTOR_H
#define FFT_DETECTOR_H

#include "loader.h"
#include "stb_fft.h"

class FFTDetector {
public:
  FFTDetector(ImgData &id);
  void run_detection();
  void make_fft_ppm(const std::string &outfile_path);

  std::vector<cmplx> get_spectrum_1d();
  std::vector<cmplx> get_spectrum_2d();

private:
  ImgData &img_;
  std::vector<float> gray_;
  std::vector<cmplx> spectrum_1d_;
  std::vector<cmplx> spectrum_2d_;

  void calculate_fft_1d();
  void calculate_fft_2d();
};

#endif  //! FFT_DETECTOR_H
