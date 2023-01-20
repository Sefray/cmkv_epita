#pragma once

#include <random>
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
  Tetravex(const Tetravex& game);

  Tile& get_tile(int x, int y) const;
  void  swap_tiles(int a, int b);

  void display() const;
  void write(std::string file_out) const;
};

void solve(Tetravex& game, bool verbose = false);

// Used for solving
float                 get_error(const Tetravex& game, const std::vector<std::pair<int, enum Direction>>& unique_values);
const std::tuple<int, int>& get_move(const Tetravex& game, const std::vector<std::tuple<int, int>>& moves, std::mt19937& mt);

std::vector<float> get_distributions(const Tetravex& game, std::vector<std::tuple<int, int>> moves);

std::tuple<int, int, enum Direction> get_tile_position_with_value(const Tetravex& game, int value);
float                                get_error_tile(const Tetravex& game, int x, int y);
float                                get_error_tile_index(const Tetravex& game, int index);

// For testing purpose
int get_error_count(const Tetravex& game);
