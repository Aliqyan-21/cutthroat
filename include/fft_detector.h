#ifndef FFT_DETECTOR_H
#define FFT_DETECTOR_H

#include "loader.h"
#include "stb_fft.h"

class FFTDetector {
public:
  FFTDetector(const ImgData &id);
  void run_detection();
  void calculate_fft();
  void make_fft_ppm(const std::string &outfile_path);
  void calculate_aaps(const std::string &outfile_path = "");
  void fit_power_law();

  std::vector<cmplx> get_spectrum_1d();
  std::vector<cmplx> get_spectrum_2d();
  std::vector<float> get_aaps();
  float get_b1();
  float get_b2();

private:
  const ImgData &img_;
  int fft_width_, fft_height_;
  std::vector<float> gray_;
  std::vector<cmplx> spectrum_1d_;
  std::vector<cmplx> spectrum_2d_;
  std::vector<float> aaps_;
  float b1_, b2_; /* magnitude, rate */

  void calculate_fft_1d();
  void calculate_fft_2d();
};

#endif  //! FFT_DETECTOR_H
