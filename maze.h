// Created by Wei Shi
// Acyclic maze generator and solver
#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <forward_list>
#include <queue>
#include <unordered_map>
#include <utility>
#include <random>
#include "image.h"
#include "disjoint_set.h"
#include "utility_methods.h"

using namespace image;

class Cell {
  public:
    Cell() {
      walls_ = vector<bool>(wall_count_, true);
    }
    Cell(const vector<bool>& walls) {
      if(walls.size() == 2) {
        walls_ = walls;
      }
    }
    Cell(vector<bool>&& walls) {
      if(walls.size() == 2) {
        walls_ = std::move(walls);
      }
    }
    bool HasRightWall() const {
      return walls_[0];
    }
    bool HasBottomWall()const {
      return walls_[1];
    }
    unsigned int get_wall_count() const {
      return wall_count_;
    }
    vector<bool> get_walls() const {
      return walls_;
    }
    void Break(unsigned int i) {
      walls_[i%wall_count_] = false;
    }
    void Print() const {
      printf("R:%d B:%d", walls_[0],walls_[1]);
    }

  private:
    const static unsigned int wall_count_ = 2;
    vector<bool> walls_; // true means wall is there
};

/**
* MAZE ALGORITHM:
*   1. Create a one dimensional grid of isolated cells.
*   2. Randomly select a wall from a randomized pool of walls to break down if
*      it separates two cells from different sets.
*   3. Union the two cells from (2) into the same set.
*   4. Remove that wall from the pool and repeat from 2 to 4 until all cells
*      are in the same set.
*
*   Entry point for maze generation is non-class method GenerateMaze().
*/
class Maze {
  public:
    Maze() {}
    // @param rows determine the number of cells for the width of the maze.
    // @param cols determing the number of cells for the height of the maze.
    explicit Maze(const size_t& rows, const size_t& cols);
    explicit Maze(size_t&& rows, size_t&& cols);
    Maze& operator=(const Maze& rhs);

    // Randomly generates the maze and stores it in cells_.
    // Generates randomized walls from cells_ and stores in in walls_.
    void Generate();
    void PrintCells() const;
    void PrintWalls() const;
    void PrintSet() const;

    // @param scale in pixels of size of each cell.
    // @return scaled .pgm grayscale image of the maze.
    Image* get_image(const size_t& scale = 15);
    Image* get_solved_image(size_t start_row, size_t start_col,
                            size_t end_row,size_t end_col, size_t scale);
    Image* get_solved_image(size_t start, size_t end, size_t scale);
    Image* get_grid(size_t rows, size_t columns, size_t scale);
    // Breadth first search solution to maze.
    // size_t template parameter is the index of the cell in cells_ vector.
    // @param start is the index of starting cell in cells_ vector.
    // @param end is the index of ending cell in cells_ vector.
    // @return an ordered list of indices to follow to reach from start to end
    // in the maze.
    forward_list<size_t> Solve(size_t start, size_t end) const;

  private:
    // @param current is the current cell's index.
    // @param wall is the wall index [0,1] = [right, bottom]
    // @return index of neighboring cell separated by the wall
    size_t GetNeighborIndex(const size_t& current, const unsigned int& wall);
    // @param i is the current cell index.
    // @param wall is the wall index [0,1] = [right, bottom]
    void UnionCells(size_t i, unsigned int wall);
    void BreakWall(size_t cell_index, size_t neighbor, unsigned int wall);
    // @param random_indices are randomized indices from indices of walls_.
    void BreakWalls(const vector<size_t>& random_indices);
    void InitializeWalls();
    void InitializeGraph();
    // @param (i,j) are the scaled indices of the scaled image.
    // @param scale is the scale.
    // sets the value of a scale*scale pixel area to white starting at (i,j).
    void SetScaledPixel(const size_t& i, const size_t& j, const size_t& scale,
                        Image* maze, const unsigned int& value);
    // Opens a path through selected wall between two cells in the image "maze".
    // @param wall is the wall index [0,1] = [right, bottom]
    // @param (i,j) is the current cell's index
    // @param maze is the image of the maze to mutate.
    // @param scale is the length in pixels of a square cell.
    void BreakWallImage(  unsigned int wall, // which side of the cell to break
                          size_t i, // cell row index
                          size_t j, // cell column index
                          Image* maze,
                          size_t scale);
    // @param current is the current cell index.
    // @param is a neigboring cell index.
    // @return true if current and neighbor are in same set.
    bool IsInSameSet(size_t current, size_t neighbor);

    DisjSets set_; // one dimensional set for storing "connectedness" of cells
    vector<Cell> cells_; // one dimensional representation of a maze.
    vector<pair<size_t, unsigned int>> walls_; // <index, wall number> pairs
    size_t num_rows_; // total number of rows of cells
    size_t num_columns_; // total number of columns of cells
};

void GenerateMaze(  const string& scale_string,
                    const string& rows_string,
                    const string& columns_string,
                    const string& unsolved_output,
                    const string& start_row_string,
                    const string& start_col_string,
                    const string& end_row_string,
                    const string& end_col_string,
                    const string& solved_output);

void GenerateMaze(  const string& scale_string,
                    const string& rows_string,
                    const string& columns_string,
                    const string& unsolved_output);

void GenerateMaze();

void GenerateGrid(const string& scale_string,
                  const string& rows_string,
                  const string& columns_string,
                  const string& output);
void GenerateGrid();

#endif