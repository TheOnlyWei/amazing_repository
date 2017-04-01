ACYCLIC MAZE GENERATOR AND SOLVER
by Wei Shi

This program was built using Linux and BASH.

INSTALL MAKE
$sudo apt-get install build-essential

COMPILE PROGRAM
1.  Go (cd ...) into the project root directory
2.  Run make commands to make all programs:
      $make all
      If you only want main maze program:
        $make create_maze
      If you also want to see how the cells are arranged in the image:
        $make create_grid

WARNING: Don't make any of the dimensions too large or it will take forever
  to generate the maze. A 15*15 pixeled cell maze with 50 cell rows and 50 cell
  columns takes up about 2.3 megabytes and has resolution 1515*1515 pixels.

CREATE GRID
  The grid is to show how the cells are arranged. The cells are indexed by the
  white squares, not including the spaces between them, which are considered walls
  even when they are knocked down to form the maze.

  WITHOUT ARGUMENTS
  $./create_grid
    You will be prompt by the CLI to enter information to generate the grid.

  WITH ARGUMENTS
  $./create_grid <1> <2> <3> <4>
    <1>:  the pixel scale or length of a square cell. Default length is 15.
    <2>:  number of rows of cells not including walls.
    <3>:  number of columns of cells not including walls.
    <4>:  grid output file name (.pgm grayscale image).

CREATE MAZE
  WITHOUT ARGUMENTS (has error check loop)
  $./create_maze
    You will be prompt by the CLI to enter information to generate the maze.

  WITH ARGUMENTS (graceful exit on error)
  Create an unsolved maze
    $./create_maze <1> <2> <3> <4> <5>
      <1>:  the pixel scale or length of a square cell. Default length is 15.
      <2>:  number of rows of cells not including walls.
      <3>:  number of columns of cells not including walls.
      <4>:  unsolved maze output file name (.pgm grayscale image).

    e.g., $./create_maze 10 20 30 unsolved.pgm
      creates a maze where every cell is 10*10 pixels with 20 rows and 30 columns
      of cells. The unsolved maze is then written to a file named "unsolved.pgm".

  Create an unsolved maze and its solved image
    $./create_maze <1> <2> <3> <4> <5> <6> <7> <8> <9>
      <1>:  the pixel scale or length of a square cell. Default length is 15.
      <2>:  unsigned integer total number of rows of cells not including walls.
      <3>:  unsigned integer total number of columns of cells not including walls.
      <4>:  string unsolved maze output file name (.pgm grayscale image).
      <5>:  unsigned integer starting row index.
      <6>:  unsigned integer starting column index.
      <7>:  unsigned integer ending row index.
      <8>:  unsigned integer ending column index.
      <9>:  string solved maze output file name (.pgm grayscale image).

    e.g., $./create_maze 15 20 30 unsolved.pgm 5 10 15 20 solved.pgm
      creates an image of a maze where every cell is 15*15 pixels, with a total
      of 20 rows of cells and 30 columns of cells. Then it creates an unsolved
      maze image named "unsolved.pgm" and a solved maze image named "solved.pgm",
      with a path marked from (5,10) to (15,20).

UNDERSTANDING THE MAZE IMAGE
- In the solved maze image, the starting point is the second darkest cell with
  gray value 90, and the ending point is the darkest cell in the image at
  gray value 0.

- The maze image generated has a default starting cell at the top left or bottom
  right as denoted by missing wall on the boundaries of the maze. However,
  solution to the maze will accept any starting cell in the image as well as any
  ending cell in the image.

- The image of the maze are padded an extra cell every other column and row. To
  get the available 2D cells structure of the cells use ./create_grid program.
  The index of these cells start from (0,0) at the top left to
  (m-1, n-1) at the bottom right for an m*n maze. The interface accepts
  2D coordinates.
