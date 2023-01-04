#pragma once

#include <string>

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

void solve(Tetravex& game);
int  get_error_count(Tetravex& game);
