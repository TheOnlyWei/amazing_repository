// Created by Ioannis Stamos
// Modified by Wei Shi
#include <iostream>
#include <string.h>
#include "image.h"

using namespace std;

namespace image {

Image::Image(const Image &an_image){
  AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
  SetNumberGrayLevels(an_image.num_gray_levels());
  for (size_t i = 0; i < num_rows(); ++i)
    for (size_t j = 0; j < num_columns(); ++j){
      SetPixel(i,j, an_image.GetPixel(i,j));
    }
}
Image::Image(Image &&an_image) {
  AllocateSpaceAndSetSize(std::move(an_image.num_rows()), std::move(an_image.num_columns()));
  SetNumberGrayLevels(std::move(an_image.num_gray_levels()));
  for (size_t i = 0; i < num_rows(); ++i)
    for (size_t j = 0; j < num_columns(); ++j){
      SetPixel(i,j, std::move(an_image.GetPixel(i,j)));
    }
}

Image::~Image(){
  DeallocateSpace();
}

Image& Image::operator=(const Image& rhs) {
  AllocateSpaceAndSetSize(rhs.num_rows(), rhs.num_columns());

  SetNumberGrayLevels(rhs.num_gray_levels());
  for (size_t i = 0; i < num_rows(); ++i) {
    for (size_t j = 0; j < num_columns(); ++j){
      SetPixel(i,j,rhs.GetPixel(i,j));
    }
  }

  return *this;
}
Image& Image::operator=(Image&& rhs) {
  AllocateSpaceAndSetSize(std::move(rhs.num_rows()), std::move(rhs.num_columns()));
  SetNumberGrayLevels(std::move(rhs.num_gray_levels()));

  for (size_t i = 0; i < num_rows(); ++i) {
    for (size_t j = 0; j < num_columns(); ++j){
      SetPixel(i,j,std::move(rhs.GetPixel(i,j)));
    }
  }
  return *this;
}

void Image::AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns) {
 if (pixels_ != nullptr){
    DeallocateSpace();
  }
  pixels_ = new int*[num_rows];
  for (size_t i = 0; i < num_rows; ++i)
    pixels_[i] = new int[num_columns];

  num_rows_ = num_rows;
  num_columns_ = num_columns;
}

void Image::DeallocateSpace() {
  for (size_t i = 0; i < num_rows_; i++)
    delete pixels_[i];
  delete pixels_;
  pixels_ = nullptr;
  num_rows_ = 0;
  num_columns_ = 0;
}

void Image::Fill(unsigned short value) {
  for(size_t i = 0; i < num_rows_; ++i) {
    for(size_t j = 0; j < num_columns_; ++j) {
      SetPixel(i,j,value);
    }
  }
}
bool ReadImage(const string &filename, Image *an_image) {
  if (an_image == nullptr) abort();
  FILE *input = fopen(filename.c_str(),"rb");
  if (input == 0) {
    cout << "ReadImage: Cannot open file" << endl;
    return false;
  }

  // Check for the right "magic number".
  char line[1024];
  if (fread(line, 1, 3, input) != 3 || strncmp(line,"P5\n",3)) {
    fclose(input);
    cout << "ReadImage: Expected .pgm file" << endl;
    return false;
  }

  // Skip comments.
  do
    fgets(line, sizeof line, input);
  while(*line == '#');

  // Read the width and height.
  int num_columns,num_rows;
  sscanf(line,"%d %d\n", &num_columns, &num_rows);
  an_image->AllocateSpaceAndSetSize(num_rows, num_columns);


  // Read # of gray levels.
  fgets(line, sizeof line, input);
  int levels;
  sscanf(line,"%d\n", &levels);
  an_image->SetNumberGrayLevels(levels);

  // read pixel row by row.
  for (int i = 0; i < num_rows; ++i) {
    for (int j = 0;j < num_columns; ++j) {
      const int byte=fgetc(input);
      if (byte == EOF) {
        fclose(input);
        cout << "ReadImage: short file" << endl;
        return false;
      }
      an_image->SetPixel(i, j, byte);
    }
  }

  fclose(input);
  return true;
}

bool WriteImage(const string &filename, const Image &an_image) {
  FILE *output = fopen(filename.c_str(), "w");
  if (output == 0) {
    cout << "WriteImage: cannot open file" << endl;
    return false;
  }
  const int num_rows = an_image.num_rows();
  const int num_columns = an_image.num_columns();
  const int colors = an_image.num_gray_levels();

  // Write the header.
  fprintf(output, "P5\n"); // Magic number.
  fprintf(output, "#\n");  // Empty comment.
  fprintf(output, "%d %d\n%03d\n", num_columns, num_rows, colors);

  for (int i = 0; i < num_rows; ++i) {
    for (int j = 0; j < num_columns; ++j) {
      const int byte = an_image.GetPixel(i , j);
      if (fputc(byte,output) == EOF) {
	    fclose(output);
            cout << "WriteImage: could not write" << endl;
	    return false;
      }
    }
  }

  fclose(output);
  return true;
}




}  // namespace ComputerVisionProjects
