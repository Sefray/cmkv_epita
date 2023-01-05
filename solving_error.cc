#include "tetravex.hh"

#include <tuple>
#include <vector>

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


bool is_well_positioned(int x, int y, enum Direction direction, int size)
{
  switch (direction)
  {
  case NORTH:
    return y == 0;
  case WEST:
    return x == 0;
  case EAST:
    return x == size - 1;
  case SOUTH:
    return y == size - 1;
  }
  return false;
}

int get_impossible_position(Tetravex& game, std::vector<std::pair<int, enum Direction>> unique_values)
{
  int impossible_position = 0;

  for (auto& v : unique_values)
  {
    auto position = get_tile_position_with_value(game, v.first);

    if (!is_well_positioned(std::get<0>(position), std::get<1>(position), v.second, game.size))
      impossible_position++;
  }

  return impossible_position;
}

float get_bad_place_average(Tetravex& game)
{
  float bad_place = 0;

  for (int y = 0; y < game.size; y++)
    for (int x = 0; x < game.size; x++)
      bad_place += get_error_tile(game, x, y);

  return bad_place;
}

float get_error(Tetravex& game, std::vector<std::pair<int, enum Direction>> unique_values)
{
  int   error_count         = get_error_count(game);
  int   impossible_position = get_impossible_position(game, unique_values);
  float bad_place           = get_bad_place_average(game);

  int   error  = error_count + impossible_position * 10;
  float ferror = static_cast<float>(error) + bad_place * 5;

  return ferror;
}