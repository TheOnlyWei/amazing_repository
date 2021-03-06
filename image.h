// Created by Ioannis Stamos
// Modified by Wei Shi
#ifndef COMPUTER_VISION_IMAGE_H_
#define COMPUTER_VISION_IMAGE_H_

#include <string>

namespace image{

class Image {
 public:
  Image(): num_rows_{0}, num_columns_{0},
	   num_gray_levels_{0}, pixels_{nullptr} {}
  explicit Image(const Image &an_image);
  explicit Image(Image &&an_image);

  ~Image();
  Image& operator=(const Image& rhs);
  Image& operator=(Image&& rhs);

  // Sets the size of the image to the given
  // height (num_rows) and columns (num_columns).
  void AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns);
  void SetNumberGrayLevels(unsigned short gray_levels) {
    num_gray_levels_ = gray_levels;
  }
  void Fill(unsigned short value);
  // Sets the pixel in the image at row i and column j
  // to a particular gray_level.
  void SetPixel(size_t i, size_t j, unsigned short gray_level) {
    if (i >= num_rows_ || j >= num_columns_) abort();
    pixels_[i][j] = gray_level;
  }

  size_t num_rows() const { return num_rows_; }
  size_t num_columns() const { return num_columns_; }
  size_t num_gray_levels() const { return num_gray_levels_; }

  int GetPixel(size_t i, size_t j) const {
    if (i >= num_rows_ || j >= num_columns_) abort();
    return pixels_[i][j];
  }

 private:
  void DeallocateSpace();
  size_t num_rows_;
  size_t num_columns_;
  unsigned short num_gray_levels_;
  int **pixels_;
};

// Reads a pgm image from file input_filename.
// an_image is the resulting image.
// Returns true if  everyhing is OK, false otherwise.
bool ReadImage(const std::string &input_filename, Image *an_image);

// Writes image an_iamge into the pgm file output_filename.
// Returns true if  everyhing is OK, false otherwise.
bool WriteImage(const std::string &output_filename, const Image &an_image);


}  // namespace ComputerVisionProjects

#endif  // COMPUTER_VISION_IMAGE_H_
