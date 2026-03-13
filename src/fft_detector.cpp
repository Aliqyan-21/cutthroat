#define STB_FFT_IMPLEMENTAION
#include "fft_detector.h"

#include <algorithm>
#include <cmath>
#include <fstream>

#include "preprocessing.h"

FFTDetector::FFTDetector(const ImgData &id) : img_(id) {
  /* this step is necessary to make width and height even number
   * as stb_fft does not work on odd values */
  fft_width_  = img_.width % 2 == 0 ? img_.width : img_.width + 1;
  fft_height_ = img_.height % 2 == 0 ? img_.height : img_.height + 1;

  /* resizing of spectrum_1d_ and spectrum_2d_ */
  spectrum_1d_.resize(fft_width_ * fft_height_);
  spectrum_2d_.resize(fft_width_ * fft_height_);

  /* forming flattened gray scale vector */
  int total = fft_width_ * fft_height_;
  gray_.assign(total, 0.0f);
  auto gray = make_grayscale(img_);
  for (int y{0}; y < img_.height; ++y) {
    for (int x{0}; x < img_.width; ++x) {
      gray_[y * fft_width_ + x] = gray[y][x];
    }
  }

  aaps_.resize(fft_width_ / 2);
}

/* _____________________ */
/* Core Public functions */
/* --------------------- */
void FFTDetector::run_detection() {
  // todo: implement some algorithm to detect
  // if the given image is real or ai.
}

/* calculates 1d and 2d ffts using utility functions:
 * - calculate_fft_1d
 * - calculate_fft_2d
 */
void FFTDetector::calculate_fft() {
  calculate_fft_1d();
  calculate_fft_2d();
}

/* make a ppm file for the fft spectrum generated */
void FFTDetector::make_fft_ppm(const std::string &outfile_path) {
  /* for each pixel in spectrum:
   * - mirror using hermitian symmetry (r2c only fills w/2+1 cols)
   *   because if not done we get image half black
   * - compute log of magnitude and save to fft-shifted destination
   * - so DC move from corner to center
   */
  std::vector<float> mag(fft_width_ * fft_height_);
  for (int y{0}; y < fft_height_; ++y) {
    for (int x{0}; x < fft_width_; ++x) {
      int sx       = (x + fft_width_ / 2) % fft_width_;
      int sy       = (y + fft_height_ / 2) % fft_height_;
      int target_x = (x <= fft_width_ / 2) ? x : fft_width_ - x;

      auto &c = spectrum_2d_[y * fft_width_ + target_x];
      float m = std::sqrt(c.real * c.real + c.imag * c.imag);

      mag[sy * fft_width_ + sx] = std::log(m);
    }
  }

  /* normalize log to [0,255] for image */
  auto [mn, mx] = std::minmax_element(mag.begin(), mag.end());
  float range   = *mx - *mn + 1e-6f;

  /* write result in ppm format */
  std::ofstream out(outfile_path, std::ios::binary);
  out << "P6\n" << fft_width_ << " " << fft_height_ << "\n255\n";
  for (float m : mag) {
    uint8_t p     = static_cast<uint8_t>(255.0f * (m - *mn) / range);
    char pixel[3] = {(char)p, (char)p, (char)p};
    out.write(pixel, 3);
  }
}

/*
 * the function make 1d radial distribution called Azimuthal Average Power
 * Spectrum (AAPS) from 2d fft spectrum by averaging the magnitude of all pixels
 * located within concentring rings. resulting AAPS vector provide `freq
 * signature` of image.
 *
 * if outfile_path is provided the AAPS vector is written into
 * a gnuplot friendly file to be plotted with gnuplot script.
 */
void FFTDetector::calculate_aaps(const std::string &outfile_path) {
  int num_rings = fft_width_ / 2;

  std::vector<int> counts(num_rings, 0);  // count of pixel

  /* now, max possible distance from center to corner */
  float kr_max =
    std::sqrt(fft_width_ * fft_width_ +
              fft_height_ * fft_height_); /* kr_max = sqrt(cx^2 * cy^2) */

  for (int y{0}; y < fft_height_; ++y) {
    for (int x{0}; x < fft_width_; ++x) {
      /* normalized ([0.0,1.0]) radial distance kr = sqrt(dx^2 + dy^2)/kr_max */
      float kr = std::sqrt(x * x + y * y) / kr_max;

      int ring = static_cast<int>(kr * (num_rings - 1));
      if (ring >= num_rings) { continue; }

      /* magnitude of complex frequency point
       * mag = sqrt(real^2 + imaginary^2);
       */
      float re = spectrum_2d_[y * fft_width_ + x].real;
      float im = spectrum_2d_[y * fft_width_ + x].imag;
      aaps_[ring] += std::sqrt(re * re + im * im);
      counts[ring]++;
    }
  }
  /* average each ring according to pixel count, ofc */
  for (int i{0}; i < num_rings; ++i) {
    if (counts[i] > 0) { aaps_[i] /= counts[i]; }
  }

  if (!outfile_path.empty()) {
    std::ofstream out(outfile_path);
    out << "# kr\t\tc(kr)\n";
    for (int i{0}; i < (int)aaps_.size(); ++i) {
      float kr = i / (num_rings - 1.0f);
      out << kr << "\t\t" << aaps_[i] << "\n";
    }
  }
}

/* _______ */
/* Getters */
/* ------- */
std::vector<cmplx> FFTDetector::get_spectrum_1d() { return spectrum_1d_; }
std::vector<cmplx> FFTDetector::get_spectrum_2d() { return spectrum_2d_; }
std::vector<float> FFTDetector::get_aaps() { return aaps_; }

/* _________________ */
/* Utility Functions */
/* ----------------- */

/*
 * Horizontal Pass:
 * function transforms each `row` from spatial (pixels) to frequency (waves).
 * to generate the 1d fft spectrum
 */
void FFTDetector::calculate_fft_1d() {
  for (int y{0}; y < fft_height_; ++y) {
    STB_FFT_R2C(gray_.data() + y * fft_width_,
                spectrum_1d_.data() + y * fft_width_, fft_width_);
  }
}

/*
 * Vertical Pass:
 * transforms the results of the horizontal pass along the `columns`.
 * to generate the 2d fft spectrum
 */
void FFTDetector::calculate_fft_2d() {
  for (int i{0}; i < fft_height_ * fft_width_; ++i) {
    spectrum_2d_[i] = spectrum_1d_[i];
  }

  std::vector<cmplx> col_in(fft_height_), col_out(fft_height_);
  for (int x{0}; x < fft_width_; ++x) {
    for (int y{0}; y < fft_height_; ++y) {
      col_in[y] = spectrum_2d_[y * fft_width_ + x];
    }

    STB_FFT(col_in.data(), col_out.data(), fft_height_);

    for (int y{0}; y < fft_height_; ++y) {
      spectrum_2d_[y * fft_width_ + x] = col_out[y];
    }
  }
}
