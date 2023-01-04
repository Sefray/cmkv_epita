#include "tetravex.hh"

int get_error_count(Tetravex& game)
{
  int error_count = 0;

  for (int y = 0; y < game.size; y++)
    for (int x = 0; x < game.size; x++)
    {
      Tile& tile = game.get_tile(x, y);

      if (x > 0)
      {
        Tile& left = game.get_tile(x - 1, y);
        if (left.value[EAST] != tile.value[WEST])
          error_count++;
      }

      if (y > 0)
      {
        Tile& top = game.get_tile(x, y - 1);
        if (top.value[SOUTH] != tile.value[NORTH])
          error_count++;
      }
    }

  return error_count;
}

void solve(Tetravex& game)
{
  // TODO
}