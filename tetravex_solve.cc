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

float get_error_rate(Tetravex& game)
{
  float error_count = static_cast<float>(get_error_count(game));
  return error_count / (game.size * game.size);
}

constexpr int max_iterations = 1000;

std::vector<std::tuple<int, int>> get_all_moves(Tetravex& game)
{
  std::vector<std::tuple<int, int>> moves;

  for (int ta = 0; ta < game.size * game.size; ta++)
  {
    if (game.tiles[ta].is_locked)
      continue;

    for (int tb = ta + 1; tb < game.size * game.size; tb++)
    {
      if (game.tiles[tb].is_locked)
        continue;

      moves.push_back(std::make_tuple(ta, tb));
    }
  }

  return moves;
}

void solve(Tetravex& game)
{
  float current_error_rate = get_error_rate(game);
  for (int i = 0; i < 100000; i++)
  {
    // Get all possible moves
    auto moves = get_all_moves(game);

    // Choose a random move
    auto move = moves[rand() % moves.size()];

    // Apply the move
    game.swap_tiles(std::get<0>(move), std::get<1>(move));

    // Check if the error rate has improved
    float new_error_rate = get_error_rate(game);

    // If it has, keep the move
    if (new_error_rate < current_error_rate)
    {
      current_error_rate = new_error_rate;

      // If the error rate is 0, we're done
      if (current_error_rate == 0)
        break;

      continue;
    }

    // If it hasn't, revert the move
    game.swap_tiles(std::get<0>(move), std::get<1>(move));
  }
}