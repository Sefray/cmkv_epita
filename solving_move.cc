#include "tetravex.hh"

#include <iostream>
#include <tuple>
#include <vector>

const std::tuple<int, int>& get_a_random_move(const std::vector<std::tuple<int, int>>& moves, std::mt19937& mt)
{
  std::uniform_int_distribution<int> uni(0, moves.size() - 1);
  int                                index = uni(mt);
  auto&                              ret   = moves[index];
  return ret;
}

float get_error_tile_index(const Tetravex& game, int index)
{
  int x = index % game.size;
  int y = index / game.size;

  return get_error_tile(game, x, y);
}


float get_error_tile(const Tetravex& game, int x, int y)
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

std::vector<float> get_tiles_errors(const Tetravex& game)
{
  std::vector<float> tiles_errors;
  for (int i = 0; i < game.size * game.size; i++)
    tiles_errors.push_back(get_error_tile_index(game, i));
  return tiles_errors;
}


std::vector<float> get_distributions(const Tetravex& game, const std::vector<std::tuple<int, int>> moves)
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

  return distribution;
}


const std::tuple<int, int>& get_a_random_move_with_distribution(const Tetravex& game, const std::vector<std::tuple<int, int>>& moves,
                                                          std::mt19937& mt)
{
  std::vector<float>              distributions = get_distributions(game, moves);
  std::discrete_distribution<int> distribution(distributions.begin(), distributions.end());
  int                             random = distribution(mt);
  auto&                           ret    = moves[random];
  return ret;
}

const std::tuple<int, int>& get_move(const Tetravex& game, const std::vector<std::tuple<int, int>>& moves, std::mt19937& mt)
{
  return get_a_random_move_with_distribution(game, moves, mt);
  // return get_a_random_move(moves, mt);
}