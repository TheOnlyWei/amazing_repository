#include "maze.h"

int main(int argc, char **argv){
  if (argc == 5) {
    GenerateGrid(argv[1],argv[2],argv[3],argv[4]);
  } else if (argc == 1) {
    GenerateGrid();
  } else {
    printf("ERROR: invalid arguments, please refer to README.txt.\n");
  }
  return 0;

}
