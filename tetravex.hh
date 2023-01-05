#pragma once

#include <string>
#include <tuple>
#include <vector>

enum Direction
{
  NORTH,
  WEST,
  EAST,
  SOUTH,
};

class Tile
{
public:
  bool is_locked;
  int  value[4];
};

class Tetravex
{
public:
  int   size;
  Tile* tiles;

  Tetravex(std::string file_in);

  Tile& get_tile(int x, int y);
  void  swap_tiles(int a, int b);

  void display();
  void write(std::string file_out);
};

void solve(Tetravex& game, int max_iteration = 10000, bool verbose = false);

// Used for solving
float                       get_error(Tetravex& game, std::vector<std::pair<int, enum Direction>> unique_values);
std::tuple<int, int, float> get_move(Tetravex& game, std::vector<std::tuple<int, int>> moves);

std::vector<float> get_distributions(Tetravex& game, std::vector<std::tuple<int, int>> moves);

std::tuple<int, int, enum Direction> get_tile_position_with_value(Tetravex& game, int value);
float                                get_error_tile(Tetravex& game, int x, int y);
float                                get_error_tile_index(Tetravex& game, int index);

// For testing purpose
int get_error_count(Tetravex& game);
