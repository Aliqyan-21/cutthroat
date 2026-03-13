# FFT Image Visualization

- currently in the code I implemented the function to save the 2d spectrum formed by fft into a ppm image.
- I generated two images ai.ppm and real.ppm using ai generated image and real image respectively.

## What is this?
After computing the 2D FFT of an image, the spectrum is visualized as a grayscale PPM image.
- **Center** = DC component.
- **Edges** = high frequency content.
- **Brightness** = energy/magnitude at that frequency.

The spectrum is FFT-shifted so DC is at the center.

## Experiment

A real image was given to Gemini and it was asked to recreate the same image.
Both images are 700x525.

- `test_data/real/1.jpeg` : original real photo
- `test_data/ai/1.jpeg` : gemini recreation of the same image

### Real Image
![real image](../assets/ppm/ai.png =300x300)

### AI Generated Image
![ai generated image](../assets/ppm/real.png =300x300)

> note: ppm converted to png to show in github markdown

## Observations

- Both spectrums look **very similar** and both show a bright cross/star at center
- Differences are very subtle, slight variation in how energy decays from center,
  and minor texture differences in the high frequency region. Not quite good at detecting
  from naked eyes.

---
Based on following the paper:
> [*Fourier Spectrum Discrepancies in Deep Network Generated Images* Dzanic et al., NeurIPS 2020](https://arxiv.org/abs/1911.06465)

This seems the reason the paper proposes **azimuthal averaging + power law fitting** instead
of relying on visual inspection. The subtle differences in the high-frequency decay rate
`b1` and `b2` may still be measurable (yet to be seen).

## Next Step
- Implement `azimuthal calculation logic` to compute the 1D radial energy curve `c(kr)`
- Plot it, can use gnuplot
- Observe
