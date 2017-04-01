// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
#include <cstdio>
#include <iostream>
#include <string>
#include <forward_list>
#include "maze.h"

using namespace std;
using namespace image;

int main(int argc, char **argv){
  if (argc == 10) {
    GenerateMaze( argv[1],argv[2],argv[3],
                  argv[4],argv[5],argv[6],
                  argv[7],argv[8],argv[9]);
  } else if (argc == 5) {
    GenerateMaze(argv[1],argv[2],argv[3],argv[4]);
  } else if (argc == 1) {
    GenerateMaze();
  } else {
      printf("ERROR: invalid arguments, please refer to README.txt.\n");
      return 0;
  }

}
