#include <iostream>
#include "maze.h"
using namespace std;

Maze::Maze(const size_t& rows, const size_t& cols) {
  cells_ = vector<Cell>(rows*cols, Cell());
  num_columns_ = cols;
  num_rows_ = rows;
  set_ = DisjSets(rows*cols);
}

Maze::Maze(size_t&& rows, size_t&& cols) {
  cells_ = vector<Cell>(std::move(rows)*std::move(cols), Cell());
  num_columns_= std::move(cols);
  num_rows_ = std::move(rows);
  set_ = DisjSets(std::move(rows)*std::move(cols));
}

Maze& Maze::operator=(const Maze& rhs) {
  set_ = rhs.set_;
  cells_ = rhs.cells_;
  walls_ = rhs.walls_;
  num_columns_ = rhs.num_columns_;
  num_rows_ = rhs.num_rows_;
  return *this;
}

void Maze::Generate() {
  if(cells_.empty()) {
    cout << "Maze is empty. Please initialize its dimensions." << endl;
    return;
  }
  InitializeWalls();
  vector<size_t> wall_index_samples = SampleRandomIndex(walls_.size());

  for(size_t i = 0; i < wall_index_samples.size(); ++i) {
    BreakWalls(wall_index_samples);
  }
}

void Maze::PrintCells() const {
  for(size_t i = 0; i < cells_.size(); ++i) {
    if(i % num_columns_ == 0)
      cout << endl;
    cout << '(';
    cells_[i].Print();
    cout << ')';
  }
  cout << endl;
}

void Maze::PrintWalls() const {
  for(size_t i = 0; i < walls_.size(); ++i) {
    if(i % num_columns_ == 0)
      cout << endl;
    cout << '(' << walls_[i].first << ", " << walls_[i].second << ')';
  }
  cout << endl;
}

void Maze::PrintSet() const {
  set_.Print();
}

Image* Maze::get_image(const size_t& scale) {
  Image* maze = new Image();
  size_t unit_row_size = (2*num_rows_)+1;
  size_t unit_col_size = (2*num_columns_)+1;
  maze->AllocateSpaceAndSetSize(scale*unit_row_size, scale*unit_col_size);
  maze->SetNumberGrayLevels(255);
  maze->Fill(130);
  SetScaledPixel(0, scale, scale, maze, 255); // create starting point
  SetScaledPixel( maze->num_rows()-scale, maze->num_columns()-(2*scale),
                  scale, maze, 255); // create ending point

  for(size_t i = scale; i < maze->num_rows(); i+=2*scale){
    for(size_t j = scale; j < maze->num_columns(); j+=2*scale) {
      SetScaledPixel(i,j,scale,maze,255);
    }
  }

  size_t row, col;
  vector<bool> wall_array;
  for(size_t i = 0; i < cells_.size(); ++i) {
    wall_array = cells_[i].get_walls();
    row = i/num_columns_; // row index from cells_ vector
    col = i%num_columns_; // column index from cells_ vector
    for(size_t k = 0; k < cells_[i].get_wall_count(); ++k) {
      if(!wall_array[k]) {
        BreakWallImage(k, row, col, maze, scale);
      }
    }
  }

  return maze;
}

Image* Maze::get_solved_image(const size_t& start_row, const size_t& start_col,
                              const size_t& end_row, const size_t& end_col,
                              const size_t& scale){

    size_t start_index = (num_columns_*start_row) + start_col;
    size_t end_index = (num_columns_*end_row) + end_col;

    Image* solved_maze = get_solved_image(start_index, end_index, scale);

    size_t start_row_index = (2*start_row)+1;
    size_t start_col_index = (2*start_col)+1;
    size_t end_row_index = (2*end_row)+1;
    size_t end_col_index = (2*end_col)+1;

    SetScaledPixel(scale*start_row_index, scale*start_col_index, scale, solved_maze, 90); // create starting point
    SetScaledPixel(scale*end_row_index, scale*end_col_index, scale, solved_maze, 0); // create ending point

    return solved_maze;
}

Image* Maze::get_solved_image(const size_t& start, const size_t& end,
                              const size_t& scale) {
  if(end > cells_.size()) {
    cout << "ERROR: Maze end point out of bounds." << endl;
    return nullptr;
  }

  Image* solved_maze = get_image(scale);
  SetScaledPixel(0, scale, scale, solved_maze, 255); // create starting point
  SetScaledPixel( solved_maze->num_rows()-scale, solved_maze->num_columns()-(2*scale),
                  scale, solved_maze, 255); // create ending point

  unsigned int path_shade = 200;

  forward_list<size_t> path = Solve(start, end);
  forward_list<size_t>::const_iterator cit = path.begin();
  ++cit;
  size_t image_unit_row; // unit row start at 1 due to boundary padding
  size_t image_unit_col; // unit col start at 1 due to boundary padding
  for(const auto& index: path) {
    image_unit_row = (2*(index/num_columns_)) + 1;
    image_unit_col = (2*(index%num_columns_)) + 1;
    SetScaledPixel(scale*image_unit_row, scale*image_unit_col, scale, solved_maze, path_shade);

    if(cit != path.end()) {
      if((index - *cit) == 1) { // moving left
        SetScaledPixel(scale*image_unit_row, scale*(image_unit_col-1), scale, solved_maze, path_shade);
      } else if ((index - *cit) == num_columns_) { // moving up
        SetScaledPixel(scale*(image_unit_row-1), scale*image_unit_col, scale, solved_maze, path_shade);
      } else if ((int)(index - *cit) == -1) { // moving right
        SetScaledPixel(scale*image_unit_row, scale*(image_unit_col+1), scale, solved_maze, path_shade);
      } else if ((int)(index - *cit) == (int)(-1*num_columns_)) { // moving down
        SetScaledPixel(scale*(image_unit_row+1), scale*image_unit_col, scale, solved_maze, path_shade);
      }
    }
    if(cit != path.end()) {
      ++cit;
    }
  }

  return solved_maze;
}

Image* Maze::get_grid(const size_t& rows, const size_t& columns,
                      const size_t& scale) {
  Image* maze = new Image();
  size_t unit_row_size = (2*num_rows_)+1;
  size_t unit_col_size = (2*num_columns_)+1;
  maze->AllocateSpaceAndSetSize(scale*unit_row_size, scale*unit_col_size);
  maze->SetNumberGrayLevels(255);
  maze->Fill(130);

  for(size_t i = scale; i < maze->num_rows(); i+=2*scale){
    for(size_t j = scale; j < maze->num_columns(); j+=2*scale) {
      SetScaledPixel(i,j,scale,maze,255);
    }
  }

  return maze;
}

forward_list<size_t> Maze::Solve(const size_t& start, const size_t& end) const {
  forward_list<size_t> result;
  unordered_map<size_t, size_t> cell_to_previous;
  unordered_map<size_t, bool> visited;

  cell_to_previous[start] = start;
  queue<size_t> cells_to_visit;
  cells_to_visit.push(start);
  size_t current_cell;
  size_t left_cell, top_cell;

  while(!cells_to_visit.empty()) {
    current_cell = cells_to_visit.front();
    visited[current_cell] = true;
    cells_to_visit.pop();

    if(current_cell == end) {
      while(current_cell != start) {
        result.push_front(current_cell);
        current_cell = cell_to_previous[current_cell];
      }
      result.push_front(current_cell);
      return result;
    }

    left_cell = current_cell-1;
    top_cell = current_cell-num_columns_;

    // check if current cell is connected to the cell to its left.
    if(!visited[left_cell] && (current_cell % num_columns_) != 0 && !cells_[left_cell].HasRightWall()) {
      cells_to_visit.push(left_cell);
      cell_to_previous[left_cell] = current_cell;
    }
    // check if current cell is connected to the cell to its top.
    if(!visited[top_cell] && (current_cell / num_columns_) != 0 && !cells_[top_cell].HasBottomWall()) {
      cells_to_visit.push(top_cell);
      cell_to_previous[top_cell] = current_cell;
    }
    // check if current cell is connected to the cell to its right.
    if(!visited[current_cell+1] && !cells_[current_cell].HasRightWall()){
      cells_to_visit.push(current_cell+1);
      cell_to_previous[current_cell+1] = current_cell;
    }
    // check if current cell is connected to the cell to its bottom.
    if(!visited[current_cell+num_columns_] && !cells_[current_cell].HasBottomWall()){
      cells_to_visit.push(current_cell+num_columns_);
      cell_to_previous[current_cell+num_columns_] = current_cell;
    }
  }

  return result;
}

size_t Maze::GetNeighborIndex(const size_t& current, const unsigned int& wall) {
  size_t neighbor_cell_index;
  switch(wall) {
    case 0: // right
      neighbor_cell_index = current + 1;
      break;
    case 1: // bottom
      neighbor_cell_index = current + num_columns_;
      break;
  }
  return neighbor_cell_index;
}

bool Maze::IsInSameSet(const size_t& current, const size_t& neighbor) {
  return ( set_.Find(current) == set_.Find(neighbor) );
}

void Maze::BreakWall( const size_t& cell_index, const size_t& neighbor,
                      const unsigned int& wall) {
  cells_[cell_index].Break(wall);
  set_.UnionSets(set_.Find(cell_index), set_.Find(neighbor));
}

void Maze::BreakWalls(const vector<size_t>& random_indices) {
  size_t current_cell;
  unsigned int current_wall;
  size_t neighbor;

  for(size_t i = 0; i < random_indices.size(); ++i) {
    current_cell = walls_[random_indices[i]].first;
    current_wall = walls_[random_indices[i]].second;
    neighbor = GetNeighborIndex(current_cell, current_wall);
    if(!IsInSameSet(current_cell, neighbor)) {
      BreakWall(current_cell, neighbor, current_wall);
    }
  }
}

void Maze::InitializeWalls() {
  for(size_t i = 0; i < cells_.size(); ++i) {
    if(i % num_columns_ != num_columns_ - 1) {
      walls_.push_back(pair<size_t, unsigned int>(i,0));
    }
    if(i / num_columns_ != num_rows_ - 1) {
      walls_.push_back(pair<size_t, unsigned int>(i,1));
    }
  }
}

void Maze::SetScaledPixel(const size_t& i, const size_t& j, const size_t& scale,
                          Image* maze, const unsigned int& value) {
  size_t row_bound = i + scale;
  size_t col_bound = j + scale;

  for(size_t x = i; x < row_bound; ++x) {
    for(size_t y = j; y < col_bound; ++y) {
      maze->SetPixel(x, y, value);
    }
  }
}

void Maze::BreakWallImage(  const unsigned int& wall, // which side of the cell's wall to break
                            const size_t& i, // cell row index
                            const size_t& j, // cell column index
                            Image* maze,
                            const size_t& scale) {
  size_t image_unit_row = (2*i) + 1; // unit row start at 1 due to boundary padding
  size_t image_unit_col = (2*j) + 1; // unit col start at 1 due to boundary padding

  switch(wall){
    case 0: // right wall
      // for right wall, i (row index) doesn't change
      // add scale to j (column index)
      SetScaledPixel(scale*image_unit_row, (scale*image_unit_col)+scale, scale, maze, 255);
      break;
    case 1: // bottom wall
      // for bottom wall, j (column index) doesn't change
      // add scale to i (row index)
      SetScaledPixel((scale*image_unit_row)+scale, scale*image_unit_col, scale, maze, 255);
      break;
  }
}

void GenerateMaze(  const string& scale_string,
                    const string& rows_string,
                    const string& columns_string,
                    const string& unsolved_output,
                    const string& start_row_string,
                    const string& start_col_string,
                    const string& end_row_string,
                    const string& end_col_string,
                    const string& solved_output) {

  if( IsUnsignedNumber(scale_string) && IsUnsignedNumber(rows_string)
      && IsUnsignedNumber(columns_string) && IsUnsignedNumber(start_row_string)
      && IsUnsignedNumber(start_col_string) && IsUnsignedNumber(end_row_string)
      && IsUnsignedNumber(end_col_string)) {

    size_t start_row = StringToSizeT(start_row_string);
    size_t start_col = StringToSizeT(start_col_string);
    size_t end_row = StringToSizeT(end_row_string);
    size_t end_col = StringToSizeT(end_col_string);
    size_t scale = StringToSizeT(scale_string);
    size_t rows = StringToSizeT(rows_string);
    size_t columns = StringToSizeT(columns_string);

    Maze my_maze(rows, columns);
    my_maze.Generate();
    Image* unsolved = my_maze.get_image(scale);
    if (!WriteImage(unsolved_output, *unsolved)){
      cout << "ERROR: can't write to file " << unsolved_output << endl;
    }

    if( end_row < rows && end_col < columns &&
        start_row < rows && start_col < columns) {
      Image* solved = my_maze.get_solved_image( start_row,
                                                start_col,
                                                end_row,
                                                end_col,
                                                scale);

      if (!solved || !WriteImage(solved_output, *solved)){
        cout << "ERROR: can't write to file " << solved_output << endl;
      }
    } else {
      cout << "ERROR: starting or ending index out of bounds." << endl;
      cout << "Solved maze not generated." << endl;
    }
  } else {
    cout << "ERROR: invalid dimensions " << rows_string << " * " << columns_string << ',' << endl;
    cout << "Dimensions must be unsigned number." << endl;
  }
}

void GenerateMaze(  const string& scale_string,
                    const string& rows_string,
                    const string& columns_string,
                    const string& unsolved_output) {

  if(IsUnsignedNumber(scale_string) && IsUnsignedNumber(rows_string) && IsUnsignedNumber(columns_string)) {
    Maze my_maze(StringToSizeT(rows_string), StringToSizeT(columns_string));
    my_maze.Generate();
    Image* unsolved = my_maze.get_image(StringToSizeT(scale_string));

    if (!WriteImage(unsolved_output, *unsolved)){
      cout << "ERROR: can't write to file " << unsolved_output << endl;
    }
  } else {
    cout << "ERROR: invalid dimensions " << rows_string << " * " << columns_string << ',' << endl;
    cout << "Dimensions must be unsigned number." << endl;
  }
}

void GenerateMaze() {
  string scale_string;
  string rows_string;
  string columns_string;
  string output;

  cout << "Enter the scale (number of pixels per side of a maze cell): ";
  cin >> scale_string;
  while(!IsUnsignedNumber(scale_string)) {
    cout << "Pixel scale must be an unsigned integer. Enter again: ";
    cin >> scale_string;
  }
  cout << "Enter the row size: ";
  cin >> rows_string;
  while(!IsUnsignedNumber(rows_string)) {
    cout << "Row size must be an unsigned integer. Enter again: ";
    cin >> rows_string;
  }
  cout << "Enter the column size: ";
  cin >> columns_string;
  while(!IsUnsignedNumber(columns_string)) {
    cout << "Column size must be an unsigned integer. Enter again: ";
    cin >> columns_string;
  }
  cout << "Enter unsolved maze output file: ";
  cin>> output;

  size_t scale = StringToSizeT(scale_string);
  size_t rows = StringToSizeT(rows_string);
  size_t columns = StringToSizeT(columns_string);

  Maze my_maze(rows, columns);
  my_maze.Generate();
  Image* im = my_maze.get_image(scale);

  if (!WriteImage(output, *im)){
    cout << "Can't write to file " << output << endl;
  }

  string start_row_string;
  string start_col_string;
  string end_row_string;
  string end_col_string;
  string solved_output;

  cout << "Enter the start row index: ";
  cin >> start_row_string;
  while(!IsUnsignedNumber(start_row_string)
        || StringToSizeT(start_row_string) >= rows) {
    cout << "ERROR: Index must be an unsigned integer within bounds of maze." << endl;
    cout << "Enter again: ";
    cin >> start_row_string;
  }
  cout << "Enter the start column index: ";
  cin >> start_col_string;
  while(!IsUnsignedNumber(start_col_string)
        || StringToSizeT(start_col_string) >= columns) {
    cout << "ERROR: Index must be an unsigned integer within bounds of maze." << endl;
    cout << "Enter again: ";
    cin >> start_col_string;
  }
  cout << "Enter the end row index: ";
  cin >> end_row_string;
  while(!IsUnsignedNumber(end_row_string)
        || StringToSizeT(end_row_string) >= rows) {
    cout << "ERROR: Index must be an unsigned integer within bounds of maze." << endl;
    cout << "Enter again: ";
    cin >> end_row_string;
  }
  cout << "Enter the end column index: ";
  cin >> end_col_string;
  while(!IsUnsignedNumber(end_col_string)
        || StringToSizeT(end_col_string) >= columns) {
    cout << "ERROR: Index must be an unsigned integer within bounds of maze." << endl;
    cout << "Enter again: ";
    cin >> end_col_string;
  }
  cout << "Enter the solved maze output: ";
  cin >> solved_output;
  size_t start_row = StringToSizeT(start_row_string);
  size_t start_col = StringToSizeT(start_col_string);
  size_t end_row = StringToSizeT(end_row_string);
  size_t end_col = StringToSizeT(end_col_string);

  if( end_row < rows && end_col < columns &&
      start_row < rows && start_col < columns) {
    Image* solved = my_maze.get_solved_image( start_row,
                                              start_col,
                                              end_row,
                                              end_col,
                                              scale);
    if (!solved || !WriteImage(solved_output, *solved)){
      cout << "ERROR: can't write to file " << solved_output << endl;
    }
  } else {
    cout << "ERROR: starting or ending index out of bounds." << endl;
    cout << "Solved maze not generated." << endl;
  }
}

void GenerateGrid(const string& scale_string,
                  const string& rows_string,
                  const string& columns_string,
                  const string& output) {

  if( IsUnsignedNumber(scale_string) && IsUnsignedNumber(rows_string)
      && IsUnsignedNumber(columns_string)) {
    size_t scale = StringToSizeT(scale_string);
    size_t rows = StringToSizeT(rows_string);
    size_t columns = StringToSizeT(columns_string);

    Maze my_maze(rows, columns);
    Image* grid = my_maze.get_grid(rows, columns, scale);

    if (!WriteImage(output, *grid)){
      cout << "ERROR: can't write to file " << output << endl;
    }
  } else {
    cout << "ERROR: invalid dimensions " << rows_string << " x " << columns_string << ',' << endl;
    cout << "Dimensions must be unsigned number." << endl;
  }
}

void GenerateGrid() {
  string scale_string;
  string rows_string;
  string columns_string;
  string output;

  cout << "Enter the scale (number of pixels per side of a maze cell): ";
  cin >> scale_string;
  while(!IsUnsignedNumber(scale_string)) {
    cout << "Pixel scale must be an unsigned integer. Enter again: ";
    cin >> scale_string;
  }
  cout << "Enter the row size: ";
  cin >> rows_string;
  while(!IsUnsignedNumber(rows_string)) {
    cout << "Row size must be an unsigned integer. Enter again: ";
    cin >> rows_string;
  }
  cout << "Enter the column size: ";
  cin >> columns_string;
  while(!IsUnsignedNumber(columns_string)) {
    cout << "Column size must be an unsigned integer. Enter again: ";
    cin >> columns_string;
  }
  cout << "Enter grid output file: ";
  cin>> output;

  size_t scale = StringToSizeT(scale_string);
  size_t rows = StringToSizeT(rows_string);
  size_t columns = StringToSizeT(columns_string);

  Maze my_maze(rows, columns);
  Image* grid = my_maze.get_grid(rows, columns, scale);

  if (!WriteImage(output, *grid)){
    cout << "Can't write to file " << output << endl;
  }

}
