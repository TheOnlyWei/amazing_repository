#ifndef UTILITY_METHODS_H
#define UTILITY_METHODS_H

#include <vector>
#include <random>
#include <string>
using namespace std;

vector<size_t> SampleRandomIndex(size_t size);
bool IsUnsignedNumber(const string& s);
size_t StringToSizeT(string s);

#endif
