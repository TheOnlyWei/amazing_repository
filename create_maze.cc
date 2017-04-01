#include "maze.h"

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
  }
  return 0;

}
