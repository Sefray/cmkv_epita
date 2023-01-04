#include <string>

enum Direction
{
  NORTH,
  EAST,
  SOUTH,
  WEST
};

class Tetravex
{
  class Tile
  {
  public:
    bool is_locked;
    int  value[4];
  };

private:
  Tile* tiles;
  int   size;

public:
  Tetravex(std::string);

  void resolve();
  void write(std::string);
  void display(std::string);
};
