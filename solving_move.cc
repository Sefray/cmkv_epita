#include "tetravex.hh"

#include <iostream>
#include <tuple>
#include <vector>

std::tuple<int, int, float> get_a_random_move(std::vector<std::tuple<int, int>> moves)
{
  int index = rand() % moves.size();

  std::tuple<int, int, float> ret = {std::get<0>(moves[index]), std::get<1>(moves[index]), 1 / (float)moves.size()};

  return ret;
}

float get_error_tile_index(Tetravex& game, int index)
{
  int x = index % game.size;
  int y = index / game.size;

  return get_error_tile(game, x, y);
}


float get_error_tile(Tetravex& game, int x, int y)
{
  Tile& tile = game.get_tile(x, y);

  if (tile.is_locked)
    return 0.0f;

  int ctile_possible_place = 0;
  int ctile_bad_place      = 0;

  if (x > 0)
  {
    ctile_possible_place++;
    Tile& left = game.get_tile(x - 1, y);
    if (left.value[EAST] != tile.value[WEST])
      ctile_bad_place++;
  }

  if (y > 0)
  {
    ctile_possible_place++;
    Tile& top = game.get_tile(x, y - 1);
    if (top.value[SOUTH] != tile.value[NORTH])
      ctile_bad_place++;
  }

  if (y < game.size - 1)
  {
    ctile_possible_place++;
    Tile& bottom = game.get_tile(x, y + 1);
    if (bottom.value[NORTH] != tile.value[SOUTH])
      ctile_bad_place++;
  }

  if (x < game.size - 1)
  {
    ctile_possible_place++;
    Tile& right = game.get_tile(x + 1, y);
    if (right.value[WEST] != tile.value[EAST])
      ctile_bad_place++;
  }

  return static_cast<float>(ctile_bad_place) / static_cast<float>(ctile_possible_place);
}

std::vector<float> get_tiles_errors(Tetravex& game)
{
  std::vector<float> tiles_errors;
  for (int i = 0; i < game.size * game.size; i++)
    tiles_errors.push_back(get_error_tile_index(game, i));

  // Display all tiles errors
  // for (int i = 0; i < tiles_errors.size(); i++)
  //   std::cout << "tile : " << i << " error : " << tiles_errors[i] << std::endl;

  return tiles_errors;
}


std::vector<float> get_distributions(Tetravex& game, std::vector<std::tuple<int, int>> moves)
{
  std::vector<float> tiles_errors = get_tiles_errors(game);

  std::vector<float> distribution;
  float              sum = 0.0f;

  for (auto& move : moves)
  {
    float error_move = tiles_errors[std::get<0>(move)] * tiles_errors[std::get<1>(move)] + (1.f / 32.0f);
    sum += error_move;
    distribution.push_back(error_move);
  }

  for (auto& error : distribution)
    error /= sum;

  // Display distribution
  // for (int i = 1; i < distribution.size(); i++)
  // {
  //   std::cout << "move : " << std::get<0>(moves[i]) << " " << std::get<1>(moves[i]) << " proba : " << distribution[i]
  //             << std::endl;
  // }

  return distribution;
}


std::tuple<int, int, float> get_a_random_move_with_distribution(Tetravex& game, std::vector<std::tuple<int, int>> moves)
{
  std::vector<float> distribution = get_distributions(game, moves);

  float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  float sum2   = 0.0f;

  for (int i = 0; i < distribution.size(); i++)
  {
    sum2 += distribution[i];
    if (sum2 > random)
    {
      std::tuple<int, int, float> ret = {std::get<0>(moves[i]), std::get<1>(moves[i]), distribution[i]};
      return ret;
    }
  }

  std::tuple<int, int, float> ret = {std::get<0>(moves[0]), std::get<1>(moves[0]), distribution[0]};
  return ret;
}

std::tuple<int, int, float> get_move(Tetravex& game, std::vector<std::tuple<int, int>> moves)
{
  // // Add a possibility to return a random move
  // // if (rand() % 100 < 10)
  return get_a_random_move_with_distribution(game, moves);  // 1 2 | 2 1 | 1 1 | 2 2 | 1 2
  // return get_a_random_move(moves);
}