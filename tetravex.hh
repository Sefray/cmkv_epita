#include <string>

enum Direction
{
  NORTH,
  EAST,
  SOUTH,
  WEST
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

  void display();
  void resolve();
  void write(std::string file_out);
};
