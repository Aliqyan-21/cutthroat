#define STB_FFT_IMPLEMENTAION
#include "fft_detector.h"

#include "preprocessing.h"

FFTDetector::FFTDetector(ImgData &id) : img_(id) {
  /* this step is necessary to make width and height even number
   * as stb_fft does not work on odd values */
  img_.width  = img_.width % 2 == 0 ? img_.width : img_.width + 1;
  img_.height = img_.height % 2 == 0 ? img_.height : img_.height + 1;

  /* resizing of spectrum_1d_ and spectrum_2d_ */
  spectrum_1d_.resize(img_.width * img_.height);
  spectrum_2d_.resize(img_.width * img_.height);

  /* forming flattened gray scale vector */
  int total = img_.width * img_.height;
  gray_.assign(total, 0.0f);
  auto gray = make_grayscale(img_);
  for (int y{0}; y < img_.height; ++y) {
    for (int x{0}; x < img_.width; ++x) {
      gray_[y * img_.width + x] = gray[y][x];
    }
  }
}

/* _____________________ */
/* Core Public functions */
/* --------------------- */
void FFTDetector::run_detection() {
  calculate_fft_1d();
  calculate_fft_2d();

  // todo: implement some algorithm to detect
  // if the given image is real or ai using the
  // fft spectrum we calculated
}

/* _______ */
/* Getters */
/* ------- */
std::vector<cmplx> FFTDetector::get_spectrum_1d() { return spectrum_1d_; }
std::vector<cmplx> FFTDetector::get_spectrum_2d() { return spectrum_2d_; }

/* _________________ */
/* Utility Functions */
/* ----------------- */

/*
 * Horizontal Pass:
 * function transforms each `row` from spatial (pixels) to frequency (waves).
 * to generate the 1d fft spectrum
 */
void FFTDetector::calculate_fft_1d() {
  for (int y{0}; y < img_.height; ++y) {
    STB_FFT_R2C(gray_.data() + y * img_.width,
                spectrum_1d_.data() + y * img_.width, img_.width);
  }
}

/*
 * Vertical Pass:
 * transforms the results of the horizontal pass along the `columns`.
 * to generate the 2d fft spectrum
 */
void FFTDetector::calculate_fft_2d() {
  for (int i{0}; i < img_.height * img_.width; ++i) {
    spectrum_2d_[i] = spectrum_1d_[i];
  }

  std::vector<cmplx> col_in(img_.height), col_out(img_.height);
  for (int x{0}; x < img_.width; ++x) {
    for (int y{0}; y < img_.height; ++y) {
      col_in[y] = spectrum_2d_[y * img_.width + x];
    }

    STB_FFT(col_in.data(), col_out.data(), img_.height);

    for (int y{0}; y < img_.height; ++y) {
      spectrum_2d_[y * img_.width + x] = col_out[y];
    }
  }
}
