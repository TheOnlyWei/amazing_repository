#include "utility_methods.h"
using namespace std;

vector<size_t> SampleRandomIndex(size_t size) {
  random_device rd;
  mt19937 gen(rd());
  vector<size_t> indices;
  for(size_t i = 0; i < size; ++i) {
    indices.push_back(i);
  }
  vector<size_t> result;
  size_t chosen_value;
  size_t current_right;
  size_t chosen_index;
  for(size_t i = 1; i <= indices.size(); ++i) {
    current_right = indices.size()-i;
    uniform_int_distribution<> dist(0,current_right);
    chosen_index = dist(gen);
    chosen_value = indices[chosen_index];
    indices[chosen_index] = indices[current_right];
    result.push_back(chosen_value);
  }
  return result;
}

bool IsUnsignedNumber(const string& s) {
  if(s.length() == 0) {
    return false;
  }

  for(size_t i = 0; i < s.length(); ++i) {
    if(s[i] < 48 || s[i] > 57) {
      return false;
    }
  }

  return true;
}

size_t StringToSizeT(string s) {
  size_t result = 0;
  size_t current_digit;
  for(size_t i = 0; i < s.length(); ++i) {
    result *= 10;
    current_digit = (size_t)(s[i] - 48);
    result += current_digit;
  }
  return result;
}
